#include "resman.h"
#include "asset.h"
#include "fontitem.h"
#include "coloritem.h"
#include "imageitem.h"
#include "strokeitem.h"
#include "effectitem.h"

namespace sia {
namespace resy {

ResGroup::~ResGroup() {
    clearRes();
}

void ResGroup::add(my_sp<ResItem> p) {
    std::cout << "+ res:" << p->key().c_str() << std::endl;
    // 资源key中必须是带有资源类型的！
    assert(p->key().contains("/"));
    res_.insert({ p->key(), p });
}

void ResGroup::clearRes() {
    for (auto i : res_) {
        i.second->releaseRes();
    }
    res_.clear();
}

my_sp<ResItem> ResGroup::rawRes(const SkString& id) {
    auto i = res_.find(id);
    if (i != res_.end()) {
        return i->second;
    }
    return nullptr;
}

#ifdef DEBUG_CODE
void ResGroup::dumpInfo() {
    for (auto i : res_) {
        std::cout << i.first.c_str() << " --> " << i.second->respath().c_str() << " | ";
    }
}
#endif

//////////////////////////////////////////////////////////////////////////
ResMan::ResMan() {
    // 我们先注册一些已知的
    regResFactory(std::make_shared<KnownResFactory<FontItem>>(this));
    regResFactory(std::make_shared<KnownResFactory<ColorItem>>(this));
    regResFactory(std::make_shared<KnownResFactory<ImageItem>>(this));
    regResFactory(std::make_shared<KnownResFactory<StrokeItem>>(this));
    regResFactory(std::make_shared<KnownResFactory<EffectItem>>(this));
}

ResMan::~ResMan() {
    clearRes();
}

void ResMan::clearRes() {
    for (auto i : all_) {
        i.second->clearRes();
    }
    all_.clear();
}

bool ResMan::parseFile(const std::string& lua_file, const std::string& user_res_parser_file) {
    if (lua_file.empty() || user_res_parser_file.empty()) {
        assert(false);
        return false;
    }

    // 运行我们自己的lua解析器
    std::string common_res_parser;
    if (!Asset::read("lua/res_parser.lua", common_res_parser)) {
        assert(false);
        setError(SkString(u8"load asset [lua/res_paresr.lua] failed!"));
        return false;
    }

    try {
        sol::state lua;
        lua.open_libraries(sol::lib::base);
        // 注册自己c++函数进去，让lua去调用
        // 发生错误的通知
        lua.set_function("host_raise_error", [=](const std::string& err) {
            setError(SkString(err.c_str()));
        });

        // 注册资源
        lua.set_function("host_reg_res", [=](const std::string& name, const std::string& typ, const std::string& key, const std::string& val) {
            // std::cout << name << typ << key << val;
            this->regResItem(SkString(name.c_str()), SkString(typ.c_str()), SkString(key.c_str()), SkString(val.c_str()));
        });
        onLuaOpen(lua);

        // 加载/检查/执行/检查结果
        auto configer_code = lua.safe_script_file(lua_file);
        if (!configer_code.valid() /*|| !code().valid()*/) {
            assert(false);
            return false;
        }

        // 先把我们的通用的资源解析器注入进去
        lua.safe_script(common_res_parser);

        // 最后再执行用户提供的自定义资源解析
        configer_code = lua.safe_script(user_res_parser_file);
        if (!configer_code.valid()/* || !code().valid()*/) {
            assert(false);
            return false;
        }
    } catch (sol::error& e) {
        setError(SkString(e.what()));
        return false;
    }

    // 等待回调
    return true;
}

my_sp<ResItem> ResMan::regResItem(const SkString& group, const SkString& typ, const SkString& key, const SkString& val) {
//     std::cout << "resman reg res: group:" << group.c_str() << ", type:" << typ.c_str()
//               << ", key:" << key.c_str() << ", val:" << val.c_str() << std::endl;

    SkString res_type = typ;
    if (res_type.isEmpty()) {
        // 所有的资源，想要解析，必须要先知道它的资源类型。
        // 如果原始的资源字符串里面没有资源类型的话，那它必须要是一个引用
        // 引用的对象中包含资源类型。即：必须先知道资源类型！
        if (!extractResType(val, res_type)) {
            return nullptr;
        }
    }

    SkString group_name, paint_item;
    if (!splitGroupPath(group, group_name, paint_item)) {
        return nullptr;
    }

    // 使用工厂机制。这样就不用在新增解析类型的时候再改这儿的代码。
    if (auto fac = factory(res_type)) {
        auto item = fac->parseRes(group_name, key, val);
        if (item && !key.isEmpty()) {
            // 只有资源id不为空的时候才需要记录起来
            // 否则资源是立即资源，创建后立即使用，然后就会销毁
            if (auto p = getGroup(group_name, true)) {
                p->add(item);
            }
        }
        return item;
    }
    std::cout << "parse res:" << res_type.c_str() << " failed!\n";
    return nullptr;
}

void ResMan::regResFactory(my_sp<IResFactory> factory) {
    factory_.insert({ factory->resType(), factory });
}

my_sp<sia::resy::IResFactory> ResMan::factory(const SkString& res_type) const {
    auto i = factory_.find(res_type);
    if (i != factory_.end()) {
        return i->second;
    }
    return nullptr;
}

my_sp<ResGroup> ResMan::getGroup(const SkString& name, bool create_if_not_exist /*= false*/) {
    auto i = all_.find(name);
    if (i != all_.end()) {
        return i->second;
    }

    if (create_if_not_exist) {
        auto p = std::make_shared<ResGroup>();
        all_.insert({ name, p });
        return p;
    }
    return nullptr;
}

bool ResMan::splitGroupPath(const SkString& prefix, SkString& name, SkString& item) const {
    if (prefix.isEmpty()) {
        return false;
    }

    // 例如 @paint/mywin/common/color/red   --> @paint/mywin
    //      @global/color/red   --> @global

    SkTArray<SkString> out;
    SkStrSplit(prefix.c_str(), "/", SkStrSplitMode::kCoalesce_SkStrSplitMode, &out);
    if (out.count() <= 1) {
        name = prefix;
        item = prefix;
        return true;
    }

    int idx = -1;
    if (out[0].equals(GLOBAL_GROUP_NAME)) {
        name = SkString(GLOBAL_GROUP_NAME);
        idx = 1;
    } else if (out[0].equals(PAINT_GROUP_NAME)) {
        name.append(PAINT_GROUP_NAME);
        name.append("/");
        name.append(out[1]);

        idx = 2;
    }

    if (idx > 0) {
        for (; idx < out.count(); ++idx) {
            item.append(out[idx]);
            item.append("/");
        }
        if (!item.isEmpty()) {
            // 去掉最后的 “/”
            item.remove(item.size() - 1, 1);
        }
    } else {
        name = prefix;
        item = prefix;
    }
    return true;
}

bool ResMan::extractResType(const SkString& resval, SkString& typ) const {
    // 只有引用才可以提取type
    if (!resval.startsWith("@")) {
        return false;
    }

    // 从val中解析
    SkTArray<SkString> out;
    SkStrSplit(resval.c_str(), "/", SkStrSplitMode::kCoalesce_SkStrSplitMode, &out);
    if (out.count() < 2) {
        return false;
    }
    typ = out[1];
    return true;
}

void ResMan::setError(const SkString& err) {
    error_msg_ = err;
    onLuaError(err);
}

#ifdef DEBUG_CODE
void ResMan::dumpInfo()const {
    for (auto i : all_) {
        std::cout << "ResManBase: " << i.first.c_str() << " : ";
        i.second->dumpInfo();
        std::cout << std::endl;
    }
}
#endif

}
}
