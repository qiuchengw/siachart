#pragma once

#include "resitem.h"

namespace sia {
namespace resy {

class RESY_API FontItem : public ResItem {
public:
    FontItem(const SkString& id = SkString(""))
        :ResItem(ResType::kResFont, SkString("font"),  id) {
    }

    ~FontItem();

    virtual bool parse(const SkString& res) override;

    static SkFontStyle fontStyle(const SkString& style, bool & found) {
        static std::unordered_map<SkString, SkFontStyle> __styles = {
            { SkString("italic"), SkFontStyle::Italic() },
            { SkString("bold"), SkFontStyle::Bold() },
            { SkString("normal"), SkFontStyle::Normal() },
            { SkString("italic-bold"), SkFontStyle::BoldItalic() },
        };

        found = false;
        auto i = __styles.find(style);
        if (i != __styles.end()) {
            found = true;
            return i->second;
        }
        return SkFontStyle();
    }

    sk_sp<SkTypeface> fontFace() const {
        return font_type_;
    }

    SkScalar fontSize()const {
        return font_size_;
    }

    const SkTArray<SkString>& fontFamilies()const {
        return font_families_;
    }

protected:
    // text size : point
    SkScalar font_size_ = 18;

    // font type
    sk_sp<SkTypeface> font_type_;

    // font family
    SkTArray<SkString> font_families_;
};

}
}
