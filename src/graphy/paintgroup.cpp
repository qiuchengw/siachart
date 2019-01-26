#include "paintgroup.h"
#include "resy/resitem.h"
#include "resy/fontitem.h"
#include "resy/coloritem.h"
#include "resy/strokeitem.h"
#include "resy/effectitem.h"

namespace sia {
namespace graphy {

bool GeometryItem::parse(const SkString& res) {
    SkTArray<SkString> out;
    SkStrSplit(res.c_str(), ",", SkStrSplitMode::kStrict_SkStrSplitMode, &out);
    if (out.count() != 4) {
        return false;
    }

    try {
        for (SkString& str : out) {
            if (str.isEmpty()) {
                str.append("0", 1); // 空的默认是0
            }
        }

        SkScalar x = std::stof(out[0].c_str());
        SkScalar y = std::stof(out[1].c_str());
        SkScalar w = std::stof(out[2].c_str());
        SkScalar h = std::stof(out[3].c_str());

        rc_.setXYWH(x, y, w, h);
        return true;
    } catch (...) {
        return false;
    }
}

class ComposedPaint {
public:
    ~ComposedPaint() {

    }

    bool applyRes(my_sp<sia::resy::ResItem> res) {
        using namespace sia::resy;
        if (!res) {
            return false;
        }

        switch (res->resType()) {
        case ResType::kResFont:
            if (auto t = reinterpret_cast<FontItem*>(res.get())) {
                paint_.setTypeface(t->fontFace());
                paint_.setTextSize(t->fontSize());
            }
            break;

        case ResType::kResColor:
            if (auto t = reinterpret_cast<ColorItem*>(res.get())) {
                paint_.setColor(t->color());
            }
            break;

        case ResType::kResEffect:
            if (auto t = reinterpret_cast<EffectItem*>(res.get())) {
                paint_.setPathEffect(t->effect());
            }
            break;

        case ResType::kResImage:
            break;

        case PaintResType::kGeometry:
            geom_ = res;
            break;

        case ResType::kResStroke:
            if (auto t = reinterpret_cast<StrokeItem*>(res.get())) {
                paint_.setStrokeWidth(t->width());
                paint_.setStrokeCap(t->cap());
                paint_.setStrokeJoin(t->join());
            }
            break;

        default:
            return false;
        }
        return true;
    }

    SkPaint & get() {
        return paint_;
    }

    bool rect(SkRect& out)const {
        if (geom_) {
            if (auto p = dynamic_cast<GeometryItem*>(geom_.get())) {
                out = p->rect();
                return true;
            }
        }
        return false;
    }

    void release() {
        geom_ = nullptr;
    }

private:
    //! geometry item
    my_sp<sia::resy::ResItem> geom_;

    // 最终的paint
    SkPaint paint_;
};

PaintGroup::PaintGroup(const SkString& id)
    :group_id_(id) {
}

PaintGroup::~PaintGroup() {

}

SkPaint* PaintGroup::getPaint(const SkString& paint_item_name, SkPaint* default_p /*= nullptr*/) {
    if (auto p = composedPaint(paint_item_name, false)) {
        return &(p->get());
    }
    return default_p;
}

my_sp<ComposedPaint> PaintGroup::composedPaint(const SkString& paint_item_name, bool create_if_not_exist /*= false*/) {
    auto i = paints_.find(paint_item_name);
    if (i != paints_.end()) {
        return i->second;
    }

    if (create_if_not_exist) {
        auto p = std::make_shared<ComposedPaint>();
        paints_.insert({ paint_item_name, p });
        return p;
    }
    return nullptr;
}

bool PaintGroup::applyRes(const SkString& paint_item_name, my_sp<sia::resy::ResItem> res) {
    auto com = composedPaint(paint_item_name, true);
    if (!com) {
        return false;
    }
    return com->applyRes(res);
}

bool PaintGroup::getGeometry(SkRect& out, const SkString& paint_item_name) {
    if (auto p = composedPaint(paint_item_name, false)) {
        return p->rect(out);
    }
    return false;
}

bool PaintGroup::adjustRawRes(SkString& res_typ, SkString& key, SkString& res_val) {
    if (key.equals(GeometryItem().resTypeStr())) {
        res_typ = key;
    }
    return true;
}

#ifdef DEBUG_CODE
void PaintGroup::dumpInfo() {
    std::cout << "\n--------------\nPaintGroup["<< group_id_.c_str() << "] built paints:" << std::endl;
    for (auto i : paints_) {
        std::cout << i.first.c_str() << " | ";
    }
    std::cout << "\n---------------" << std::endl;
}
#endif

}
}
