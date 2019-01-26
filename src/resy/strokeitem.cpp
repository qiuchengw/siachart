#include "strokeitem.h"

namespace sia {
namespace resy {

bool StrokeItem::parse(const SkString& res) {
    respath_ = res;
    if (detectRefrence(res)) {
        if (isValid()) {
            if (auto p = reinterpret_cast<StrokeItem*>(res_ref_.get())) {
                width_ = p->width();
                join_ = p->join();
                cap_ = p->cap();
                return true;
            }
        }
        return false;
    }

    SkTArray<SkString> out;
    SkStrSplit(res.c_str(), " ", SkStrSplitMode::kCoalesce_SkStrSplitMode, &out);
    if (out.empty()) {
        return false;
    }

    try {
        width_ = std::stof(out[0].c_str());

        if (out.count() > 1) {
            // cap
            cap_ = parseCap(out[1].c_str());
        }

        if (out.count() > 2) {
            join_ = parseJoin(out[2].c_str());
        }
        return true;
    } catch (...) {
        return false;
    }
}

SkPaint::Cap StrokeItem::parseCap(const char* cap) {
    struct _Caps {
        std::string ins;
        SkPaint::Cap cap;
    };
    static const std::vector<_Caps> _CAPS = {
        {"butt", SkPaint::Cap::kButt_Cap },
        {"round", SkPaint::Cap::kRound_Cap },
        {"square", SkPaint::Cap::kSquare_Cap },
    };

    for (auto i : _CAPS) {
        if (i.ins.compare(cap) == 0) {
            return i.cap;
        }
    }
    return SkPaint::Cap::kDefault_Cap;
}

SkPaint::Join StrokeItem::parseJoin(const char* join) {
    struct _Joins {
        std::string ins;
        SkPaint::Join j;
    };
    static const std::vector<_Joins> _JOINS = {
        {"miter", SkPaint::Join::kMiter_Join },
        {"round", SkPaint::Join::kRound_Join },
        {"bevel", SkPaint::Join::kBevel_Join },
    };

    for (auto i : _JOINS) {
        if (i.ins.compare(join) == 0) {
            return i.j;
        }
    }
    return SkPaint::Join::kDefault_Join;
}

}
}
