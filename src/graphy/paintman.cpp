#include "paintman.h"
#include "../util.h"
#include "resy/asset.h"

namespace sia {
namespace graphy {

using namespace sia::resy;

my_sp<PaintMan> PaintMan::inst_ = nullptr;
PaintMan::PaintMan() {

    // 不能在构造函数里面调用 shared_from_this
    // 这个时候指针还没构造好
    // inst_ = shared_from_this();

    // 注册一个geometryItem
    regResFactory(std::make_shared<KnownResFactory<GeometryItem>>(this));
}

PaintMan::~PaintMan() {
}

void PaintMan::init() {
    inst_ = shared_from_this();
}

bool PaintMan::load(const SkString& config_file) {
    // 让paint res加载
    std::string paint_parser_code;
    if (!Asset::read("lua/paint_parser.lua", paint_parser_code)) {
        return false;
    }

    // 清空已创建的paints
    paints_.clear();

    // 删除原有的资源
    clearRes();

    // 重新加载文件
    return ResMan::parseFile(config_file.c_str(), paint_parser_code);
}

my_sp<PaintGroup> PaintMan::paintGroup(const SkString& group_name, bool create_if_not_exist)  {
    // std::cout << "PaintMan: findPaintGroup:" << group_name.c_str() << std::endl;

    auto i = paints_.find(group_name);
    if (i != paints_.end()) {
        return i->second;
    }

    if (create_if_not_exist) {
        auto p = std::make_shared<PaintGroup>(group_name);
        paints_.insert({ group_name, p });
        return p;
    }
    return nullptr;
}

my_sp<PaintMan> PaintMan::instance() {
    return inst_;
}

#ifdef DEBUG_CODE
void PaintMan::dumpInfo()const {
    std::cout << "--------------\nPaintMan: registered paint config:\n";
    for (auto i : paints_) {
        // std::cout << i.first.c_str() << " | ";
        i.second->dumpInfo();
    }
    std::cout << std::endl;

    std::cout << "--------------\nPaintMan: resources:\n";
    ResMan::dumpInfo();
}
#endif

void PaintMan::onLuaOpen(sol::state& lua) {
    // 注册几个回调
    lua.set_function("build_paint", [=](const std::string& paint_item, const std::string& res_typ, const std::string& res_val) {
        this->buildPaint(SkString(paint_item.c_str()), SkString(res_typ.c_str()), SkString(res_val.c_str()));
    });
}

void PaintMan::buildPaint(const SkString& uri, const SkString& res_typ, const SkString& res_val) {
    // 构造PaintConfig
    SkString group_name, paint_item_key;
    if (!ResMan::splitGroupPath(uri, group_name, paint_item_key)) {
        return;
    }
    std::cout << "PaintMan: build paint:[" << uri.c_str() << "] group:[" << group_name.c_str()
              << "] item:[" << paint_item_key.c_str() << "]"
              << "  resType:[" << res_typ.c_str() << "]  val:[" << res_val.c_str() << "]";

    // static const SkString EMPTY_KEY("");
    if (auto group = paintGroup(group_name, true)) {
        // 我们先给paintGroup一个处理的机会。也许paintGroup会对资源信息做一些修改等。
        SkString correct_typ = res_typ;
        SkString correct_key = paint_item_key;
        SkString correct_val = res_val;
        if (group->adjustRawRes(correct_typ, correct_key, correct_val)) {
            if (auto res = ResMan::regResItem(group_name, correct_typ, correct_key, correct_val)) {
                group->applyRes(paint_item_key, res);
            } else {
                std::cout << "  parse res failed!\n";
            }
        }
    }
    std::cout << std::endl;

}

}
}