#include "mylayout.h"
#include <iostream>
#include "graphy/widget.h"


namespace test {

bool MyLayout::parse(const SkString& layout) {
    return true;
}

void dumpRect(const char* name, const SkRect& r) {
//    return;

    std::cout << name << ": [" << r.left() << " | " << r.top() << " | " << r.width() << " | " << r.height() << "]" << std::endl;
}

void MyLayout::doLayout() {
    using namespace sia::graphy;
    SkRect rc = geometry();
    dumpRect("------------------->\nrc", rc);

    SkScalar x = ZERO_SCALAR;
    SkScalar y = ZERO_SCALAR;

    const SkScalar price_width = SkIntToScalar(100);
    const SkScalar ratio_width = SkIntToScalar(100);
    const SkScalar title_height = SkIntToScalar(30);
    const SkScalar calendar_height = SkIntToScalar(30);

    // 开始布局
    Widget* price = widget("price");
    SkRect r_w = SkRect::MakeXYWH(0, 0, price_width, rc.height() - calendar_height);
    price->setGeometry(r_w);
    dumpRect("price", r_w);

    // title
    Widget* title = widget("title");
    r_w = SkRect::MakeXYWH(price_width, 0, rc.width() - price_width - ratio_width, title_height);
    title->setGeometry(r_w);
    dumpRect("title", r_w);

    // ratio
    Widget* ratio = widget("ratio");
    r_w = SkRect::MakeXYWH(rc.width() - ratio_width, 0, ratio_width, rc.height() - calendar_height);
    ratio->setGeometry(r_w);
    dumpRect("ratio", r_w);

    // calendar
    Widget* calendar = widget("calendar");
    r_w = SkRect::MakeXYWH(0, rc.height() - calendar_height, rc.width(), calendar_height);
    calendar->setGeometry(r_w);
    dumpRect("calendar", r_w);

    // panel
    Widget* panel = widget("panel");
    r_w = SkRect::MakeXYWH(price_width, title_height,
                           rc.width() - price_width - ratio_width, rc.height() - calendar_height - title_height);
    panel->setGeometry(r_w);
    dumpRect("panel", r_w);
}

}

