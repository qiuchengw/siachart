#include "viewitemtooltip.h"

namespace sia {
namespace charty {

SkScalar ROW_HEIGHT = SkIntToScalar(22);
ViewItemTooltip::ViewItemTooltip()
    :VisualItem(SkString("tooltip"), nullptr) {

}

void ViewItemTooltip::setTitle(const SkString& title) {
    reset();
    title_ = title;
    addTextRect(title_);
}

void ViewItemTooltip::addTextRect(const SkString& cont) {
    SkRect rc = SkRect::MakeEmpty();
    if (text_ && !cont.isEmpty()) {
        text_->measureText(cont.c_str(), cont.size(), &rc);

        SkScalar w = std::max(width(), (rc.width() + padding_*3));	// 加宽一点
        SkScalar h = height() + std::max(ROW_HEIGHT, rc.height());
        setSize(w, h);
    }
}

void ViewItemTooltip::addBodyItem(const SkString& cont, SkColor color) {
    SeriesTooltipData::Item item;
    item.cont = cont;
    item.color = (color == Sk_InvalidColor) ? bodyTextColor() : color;
    body_.push_back(item);

    addTextRect(cont);
}

void ViewItemTooltip::setBodyItems(const SkTArray<SeriesTooltipData::Item>& items) {
    // 重置
    for (auto& item : items) {
        addBodyItem(item.cont, item.color);
    }
}

void ViewItemTooltip::reset() {
    title_.reset();
    body_.reset();
    setSize(SkIntToScalar(0), SkIntToScalar(0));
}

void ViewItemTooltip::onDraw(SkCanvas* canvas) {
    SkRect rc = rect();
    canvas->drawRect(rc, *bk_);

    // rc = rc.makeInset(1, 1);
    rc.fBottom -= 1;
    rc.fRight -= 1; // 因为会画到外面去
    canvas->drawRect(rc, *border_);

    SkScalar y = (ROW_HEIGHT / 2) + padding_;
    // title
    if (!title_.isEmpty()) {
        canvas->drawLine(1, ROW_HEIGHT, rc.width(), ROW_HEIGHT, *border_);

        SkPaint title_paint = *text_;
        SkTextUtils::DrawString(canvas, title_, width() / 2, y, title_paint, SkTextUtils::kCenter_Align);
        y += ROW_HEIGHT;
    }

    for (auto& item : body_) {
        // 颜色
        SkPaint body_paint = *text_;
        body_paint.setColor(item.color);
        canvas->drawString(item.cont, padding_, y, body_paint);

        y += ROW_HEIGHT;
    }
}

void ViewItemTooltip::preparePaints() {
    bk_ = declarePaint(SkString("background"), SkColorSetRGB(16, 16, 66));
    border_ = declarePaint(SkString("border"), SK_ColorYELLOW);
    border_->setStyle(SkPaint::kStroke_Style);
    text_ = declarePaint(SkString("text"), SK_ColorWHITE, true);
    text_->setAntiAlias(true);
}

SkColor ViewItemTooltip::bodyTextColor() const {
    if (text_) {
        return text_->getColor();
    }
    return SK_ColorWHITE;
}

}

}

