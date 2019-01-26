#pragma once

#include "resitem.h"

namespace sia {
namespace resy {

class RESY_API EffectItem : public ResItem {
public:
    EffectItem(const SkString& id = SkString(""))
        :ResItem(ResType::kResEffect, SkString("effect"), id) {
    }

    virtual bool parse(const SkString& res) override;

    sk_sp<SkPathEffect> effect()const;

protected:
    sk_sp<SkPathEffect> effect_ = nullptr;
};

}
}
