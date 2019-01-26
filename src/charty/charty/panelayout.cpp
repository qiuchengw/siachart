#include "panelayout.h"
#include <iostream>
#include "graphy/widget.h"
#include "panel.h"

namespace sia {
namespace charty {

void dumpRect(const char* name, const SkRect& r);

void PanelLayout::setPanel(Panel* p) {
    panel_ = p;
}

bool PanelLayout::parse(const SkString& layout) {
    return true;
}

void PanelLayout::doLayout() {
    using namespace sia::graphy;
    SkRect rc = geometry();

    // 在mainview 以及value/ratio 下面加上一个padding
    static const SkScalar PADDING_BOTTOM = 10;

    PanelItem* value_axis = reinterpret_cast<PanelItem*>(widget("value_axis"));
    const SkScalar value_axis_width = value_axis ? value_axis->width() : SkIntToScalar(0);

    PanelItem* ratio_axis = reinterpret_cast<PanelItem*>(widget("ratio_axis"));
    const SkScalar ratio_width = ratio_axis ? ratio_axis->width() : SkIntToScalar(0);

    PanelItem* title = reinterpret_cast<PanelItem*>(widget("title"));
    const SkScalar title_height = title ? title->height() : SkIntToScalar(0);

    PanelItem* calendar = reinterpret_cast<PanelItem*>(widget("calendar"));
    const SkScalar calendar_height = calendar ? calendar->height() : SkIntToScalar(0);

    const SkScalar MAIN_VIEW_WIDTH = rc.width() - value_axis_width - ratio_width;
    const SkScalar MAIN_VIEW_HEIGHT = rc.height() - title_height - calendar_height;
    // 开始布局
    // title
    //! 注意：ContentArea 一定要在setGeometry生效之前设置，否则界面计算混乱
    // 因为geometry改变导致控件内容重绘。而chart的控件重绘使用的contentArea大小。
    // 这个时候contentArea不设置好就计算错误了
    SkRect rc_w = SkRect::MakeXYWH(value_axis_width, 0, MAIN_VIEW_WIDTH, title_height);
    if (title) {
        title->setContentArea(SkRect::MakeXYWH(0, 0, rc_w.width(), rc_w.height()));
        title->setGeometry(rc_w);
    }
    // title->setContentArea(SkRect::MakeXYWH(value_axis_width, 0, MAIN_VIEW_WIDTH, title_height));
    dumpRect("title", rc_w);

    // value_axis
    rc_w = SkRect::MakeXYWH(0, 0, value_axis_width, rc.height() - calendar_height);
    if (value_axis) {
        value_axis->setContentArea(SkRect::MakeXYWH(0, title_height, rc_w.width(), rc_w.height() - PADDING_BOTTOM));
        value_axis->setGeometry(rc_w);
    }
    dumpRect("value_axis", rc_w);

    // ratio_axis
    rc_w = SkRect::MakeXYWH(rc.width() - ratio_width, 0, ratio_width, rc.height() - calendar_height);
    if (ratio_axis) {
        ratio_axis->setContentArea(SkRect::MakeXYWH(0, title_height, rc_w.width(), rc_w.height() - PADDING_BOTTOM));
        ratio_axis->setGeometry(rc_w);
    }
    dumpRect("ratio_axis", rc_w);

    // calendar
    rc_w = SkRect::MakeXYWH(0, rc.height() - calendar_height, rc.width(), calendar_height);
    if (calendar) {
        calendar->setContentArea(SkRect::MakeXYWH(value_axis_width, 0, MAIN_VIEW_WIDTH, calendar_height));
        calendar->setGeometry(rc_w);
    }
    dumpRect("calendar", rc_w);

    // mainview 必须在series之前进行设置大小！
    // 否则series的坐标位置显示错误！
    PanelItem* mainview = reinterpret_cast<PanelItem*>(widget("mainview"));
    rc_w = SkRect::MakeXYWH(value_axis_width, title_height, MAIN_VIEW_WIDTH, MAIN_VIEW_HEIGHT);

    SkRect rc_cont = SkRect::MakeXYWH(0, 0, MAIN_VIEW_WIDTH, MAIN_VIEW_HEIGHT - PADDING_BOTTOM);
    if (mainview) {
        mainview->setContentArea(rc_cont);
        mainview->setGeometry(rc_w);
    }

    if (panel_) {
        // 所有的副图都覆盖在主图之上！
        for (auto p : panel_->subViews()) {
            p->setContentArea(rc_cont);
            p->setGeometry(rc_w);
        }
    }
    dumpRect("mainview", rc_w);
}

}
}
