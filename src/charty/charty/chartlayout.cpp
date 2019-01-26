#include "chartlayout.h"
#include <iostream>
#include "graphy/widget.h"
#include "panel.h"

namespace sia {
namespace charty {

void ChartLayout::setPanels(std::vector<Panel*>* panels) {
    panels_ = panels;
    doLayout();
}

bool ChartLayout::parse(const SkString& layout) {
    return true;
}

void dumpRect(const char* name, const SkRect& r) {
//    return;

    std::cout << name << ": [" << r.left() << " | " << r.top() << " | " << r.width() << " | " << r.height() << "]" << std::endl;
}

void ChartLayout::doLayout() {
    using namespace sia::graphy;

    if (!panels_ || panels_->empty()) {
        return;
    }

    SkRect rc = rect();
    const auto PANELS_COUNT = panels_->size();

    // 暂时我们固定非主panel的高度
    const SkScalar NOT_MAIN_HEIGHT = 150;
    const SkScalar MAIN_HEIGHT = (PANELS_COUNT > 1)
                                 ? (rc.height() - NOT_MAIN_HEIGHT * (PANELS_COUNT - 1))
                                 : rc.height();

    SkScalar y = 0;
    int i = 0;
    for (int i = 0; i < PANELS_COUNT; ++i) {
        SkScalar child_height = ((i > 0) ? NOT_MAIN_HEIGHT : MAIN_HEIGHT);
        SkRect rc_child = SkRect::MakeXYWH(0, y, rc.width(), child_height);
        (*panels_)[i]->setGeometry(rc_child);
        y += child_height;
    }
}

}
}
