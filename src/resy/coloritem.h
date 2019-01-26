#pragma once

#include "resitem.h"

namespace sia {
namespace resy {

class RESY_API ColorItem : public ResItem {
public:
    ColorItem(const SkString& id = SkString(""))
        :ResItem(ResType::kResColor, SkString("color"), id) {
    }

    virtual bool parse(const SkString& res) override;

    inline SkColor color()const {
        return c_;
    }

protected:
    SkColor c_;
};

}
}
