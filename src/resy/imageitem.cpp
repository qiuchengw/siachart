#include "imageitem.h"

namespace sia {
namespace resy {

bool ImageItem::parse(const SkString& res) {
    respath_ = res;
    if (detectRefrence(res)) {
        if (isValid()) {
            if (auto p = reinterpret_cast<ImageItem*>(res_ref_.get())) {
                img_ = p->image();
                return true;
            }
        }
        return false;
    }

    sk_sp<SkData> encoded = SkData::MakeFromFileName(res.c_str());
    if (encoded) {
        img_ = SkImage::MakeFromEncoded(encoded);
        return !!img_;
    }
    return false;
}

}
}
