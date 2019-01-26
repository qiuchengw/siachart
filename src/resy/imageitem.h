#pragma once

#include "resitem.h"

namespace sia {
namespace resy {

class RESY_API ImageItem : public ResItem {
public:
    ImageItem(const SkString& id = SkString(""))
        :ResItem(ResType::kResImage, SkString("image"), id) {
    }

    virtual bool parse(const SkString& res) override;

    inline SkISize size() const {
        return img_ ? img_->dimensions() : SkISize::Make(0, 0);
    }

    inline sk_sp<SkImage> image() {
        return img_;
    }

protected:
    sk_sp<SkImage> img_;
};

}
}
