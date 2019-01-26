#include "ratioaxis.h"
#include "mainview.h"
#include "panel.h"

namespace sia {

namespace charty {

RatioAxis::RatioAxis(Panel* parent)
    :Axis(SkString("ratio_axis"), parent) {

}

void RatioAxis::preparePaints() {
    bk_ = declarePaint(SkString("background"));
    text_ = declarePaint(SkString("text"), SK_ColorWHITE, true);
    above_text_ = declarePaint(SkString("above_text"), SK_ColorRED, true);
    below_text_ = declarePaint(SkString("below_text"), SK_ColorGREEN, true);
    border_ = declarePaint(SkString("border"), SK_ColorRED);

    SkRect rc;
    if (getConfigGeometry(rc)) {
        setWidth(rc.width());
    }
}

void RatioAxis::onDraw(SkCanvas* canvas) {
    auto rc = rect();
    canvas->drawRect(rc, *bk_);
    // 左边界
    canvas->drawLine(rc.left(), rc.top(), rc.left(), rc.bottom(), *border_);
    // 上边界
    if (panel()->hasItem(PanelItemType::kPanelTitle)) {
        SkScalar y = border_->getStrokeWidth() / 2;
        canvas->drawLine(0, y, width(), y, *border_);
    }

    auto& val_slice = mainView()->valSlice();
    // 没有网格线的时候才画短线
    auto &gd = panel()->grid();
    bool draw_line = (!gd.hasHorizontalLine() && chartbase::validVal(val_slice.middleVal()));

    Title* title = panel()->title();
    const SkScalar y_offset = title ? title->height() : SkIntToScalar(0);	// 要加上title的高度
    const SkScalar text_offset_y_ = textVerticalMiddle(*text_) + contentRect().top();
    SkPaint line_paint;
    line_paint.setColor(gd.gridColor());

    const SkScalar PADDING_LEFT = draw_line ? SkIntToScalar(7) : SkIntToScalar(5);
    // 从上到下画档位信息
    SkPaint *pp = nullptr;
    for (const ValueSlice::PosVal& pv : val_slice.ratioPos()) {
        if (util::isZero(pv.val)) {
            pp = text_;
        } else if (pv.val > 0) {
            pp = above_text_;
        } else {
            pp = below_text_;
        }
        canvas->drawString(pv.sval, PADDING_LEFT, pv.ypos + text_offset_y_, *pp);
        if (draw_line) {
            SkScalar y = pv.ypos + y_offset; // 要加上title的高度以适应偏移
            canvas->drawLine(2, y, 5, y, line_paint);
        }
    }
}

}

}

