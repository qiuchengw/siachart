#include "effectitem.h"
#include "SkDashPathEffect.h"

namespace sia {
namespace resy {

bool EffectItem::parse(const SkString& res) {
    respath_ = res;
    if (detectRefrence(res)) {
        if (isValid()) {
            if (auto p = reinterpret_cast<EffectItem*>(res_ref_.get())) {
                effect_ = p->effect();
                return true;
            }
        }
        return false;
    }

    SkTArray<SkString> out;
    SkStrSplit(res.c_str(), ":", SkStrSplitMode::kCoalesce_SkStrSplitMode, &out);
    if (out.count() != 2) {
        return false;
    }

    if (out[0].equals("dashed")) {	// dashed effect
        SkTArray<SkString> para;
        SkStrSplit(out[1].c_str(), " ", SkStrSplitMode::kCoalesce_SkStrSplitMode, &para);

        SkTArray<SkScalar> intervals;
        for (auto &s : para) {
            intervals.push_back(std::stof(s.c_str()));
        }

        // ´´½¨effect
        effect_ = SkDashPathEffect::Make(intervals.begin(), intervals.count(), SkIntToScalar(0));
        return true;
    }
    return false;
}

sk_sp<SkPathEffect> EffectItem::effect() const {
    return effect_;
}


}
}
