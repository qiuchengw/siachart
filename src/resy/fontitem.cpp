#include "fontitem.h"

namespace sia {
namespace resy {

FontItem::~FontItem() {
}

bool FontItem::parse(const SkString& res) {
    respath_ = res;
    if (detectRefrence(res)) {
        if (isValid()) {
            if (auto p = reinterpret_cast<FontItem*>(res_ref_.get())) {
                font_size_ = p->fontSize();
                font_type_ = p->fontFace();
                font_families_ = p->fontFamilies();
                return true;
            }
        }
        return false;
    }

    // "italic bold 12px/20px arial,sans-serif;"
    SkTArray<SkString> out;
    SkStrSplit(res.c_str(), " ", SkStrSplitMode::kCoalesce_SkStrSplitMode, &out);
    if (out.count() < 2) {
        return false;
    }

    int len = out.count();
    bool found = false;
    int idx = 0;
    // 解析字体Style
    SkFontStyle font_style = fontStyle(out[idx], found);
    if (found) {
        idx++;
    }

    // 解析大小
    font_size_ = SkIntToScalar(16);
    if (idx < len) {
        try {
            font_size_ = SkIntToScalar(std::stoi(out[idx].c_str()));
        } catch (...) {
            return false;
        }
        idx++;
    }

    // 解析字family
    SkString font_faimily;
    if (idx < len) {
        SkStrSplit(out[idx].c_str(), ",", SkStrSplitMode::kCoalesce_SkStrSplitMode, &font_families_);
        if (font_families_.count() < 1) {
            return false;
        }
        // 暂时永远都是第一个有效！
        // TODO: 支持自动选择一个有用的
        font_faimily = *font_families_.begin();
    }
    font_type_ = SkTypeface::MakeFromName(font_faimily.isEmpty() ? nullptr : font_faimily.c_str(), font_style);

    return true;
}

}
}
