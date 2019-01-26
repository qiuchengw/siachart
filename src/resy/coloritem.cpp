#include "coloritem.h"

namespace sia {
namespace resy {

bool ColorItem::parse(const SkString& res) {
    respath_ = res;
    if (detectRefrence(res)) {
        if (isValid()) {
            if (auto p = reinterpret_cast<ColorItem*>(res_ref_.get())) {
                c_ = p->color();
                return true;
            }
        }
        return false;
    }

    // #RGBA --> #00ff0011
    if (!res.startsWith('#') || res.size() < 7) {
        return false;
    }

    const char *p = res.c_str();
    char rr[3] { 0 }, gg[3] { 0 }, bb[3] { 0 }, aa[3] { 0 };
    strncpy(rr, p + 1, 2);
    strncpy(gg, p + 3, 2);
    strncpy(bb, p + 5, 2);
    if (res.size() >= 9) {
        strncpy(aa, p + 7, 2);
    } else {
        strncpy(aa, "ff", 2);   // 默认是不透明的！
    }

    try {
        int r = std::stoi(rr, nullptr, 16);
        int g = std::stoi(gg, nullptr, 16);
        int b = std::stoi(bb, nullptr, 16);
        int a = std::stoi(aa, nullptr, 16);

        c_ = SkColorSetARGB(a, r, g, b);
        return true;
    } catch (...) {
        return false;
    }
}

}
}
