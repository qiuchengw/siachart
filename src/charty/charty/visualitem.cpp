#include "visualitem.h"
#include "graphy/paintman.h"
#include "chartman.h"

namespace sia {

namespace charty {

VisualItem::VisualItem(const SkString& name, graphy::Widget* parent)
    :graphy::Widget(name, parent) {

    setVisibleP(true);
}

bool VisualItem::ready() const {
    if (!paint_ready_) {
        return false;
    }

    if (auto ds = datasource()) {
        return ds->ready();
    }
    return false;
}

void VisualItem::setChartContext(ChartContext* t) {
    ctx_ = t;
    forVisualChildren([=](VisualItem* c) {
        c->setChartContext(t);
        return true;
    });
    refreshPaints();
}

Dict* VisualItem::dictionary() const {
    return ChartMan::instance()->dict();
}

void VisualItem::preparePaints() {
//     forVisualChildren([](VisualItem* c) {
//         c->preparePaints();
//         return true;
//     });
}

SkScalar VisualItem::textVerticalMiddle(const SkPaint& paint, SkScalar height) {
    SkRect rc = textRect(SkString(u8"159中文"), paint);
    return (height + rc.height()) / 2;
}

SkRect VisualItem::textRect(const SkString& str, const SkPaint& paint,
                            const SkPoint& left_top /*= SkPoint::Make(SkIntToScalar(0), SkIntToScalar(0))*/) {
    SkRect rc;
    paint.measureText(str.c_str(), str.size(), &rc);
    rc.offsetTo(left_top.x(), left_top.y());
    return rc;
}

void VisualItem::drawMultilineText(SkCanvas* canvas, const SkPaint& paint,
                                   const SkString& text, const SkRect& rc, SkScalar spacing) {
    SkTArray<SkString> lines;
    SkStrSplit(text.c_str(), "\n", SkStrSplitMode::kStrict_SkStrSplitMode, &lines);

    if (lines.empty()) {
        return;
    }

    // 检测高度
    const SkScalar LINE_MIDDLE = textVerticalMiddle(paint);
    const SkScalar LINE_HEIGHT = LINE_MIDDLE * 2 + spacing;

    SkScalar y = LINE_HEIGHT + rc.top();
    for (SkString& line : lines) {
        canvas->drawString(line, rc.left() + spacing, y, paint);
        y += LINE_HEIGHT;
    }
}

void VisualItem::forVisualChildren(std::function<bool(VisualItem*)> cb) {
    if (!cb) {
        return;
    }

    for (auto *w : children()) {
        if (VisualItem* v = dynamic_cast<VisualItem*>(w)) {
            if (!cb(v)) {
                break;
            }
        }
    }
}

SkPaint* VisualItem::declarePaint(const SkString& name, SkColor def_color /*= SK_ColorWHITE*/,
                                  bool antialias /*= false*/) {
    if (painters_) {
        if (auto ret = painters_->getPaint(name)) {
            return ret;
        }
    }

    // 在我们的缓存中查找一个同名的
    auto i = paints_.find(name);
    if (i != paints_.end()) {
        return i->second.get();
    }

    // 新创建一个
    my_sp<SkPaint> new_p = std::make_shared<SkPaint>();
    new_p->setColor(def_color);
    new_p->setAntiAlias(antialias);
    // new_p->setTextAlign(textAlign);
    paints_.insert({name, new_p});
    return new_p.get();
}

bool VisualItem::getConfigGeometry(SkRect& rc) {
    if (painters_) {
        return painters_->getGeometry(rc);
    }
    return false;
}

bool VisualItem::onEvent(const SkEvent& evt) {
    bool handled = false;
    if (evt.isType(ChartEventType::kSiaSeriesStyleChanged)
            || evt.isType(ChartEventType::kSiaThemeChanged)) {
        refreshPaints();
        handled = true;
    }
    return handled;
}

void VisualItem::refreshPaints() {
    // 一定要先将之前的引用清除掉，可以释放内存！
    painters_ = nullptr;

    // 设置正在准备paint
    paint_ready_ = false;
    // 清空之前创建的临时paint
    paints_.clear();

    // 找到自己对应的painter组
    if (auto man = sia::graphy::PaintMan::instance()) {
        SkString group_name("@paint/");
        group_name.append(name());
        painters_ = man->paintGroup(group_name, false);
    }

    // 让派生类准备自己的paints
    preparePaints();

    // 此时，才可以真正的开始界面的绘制
    paint_ready_ = true;
}

}
}

