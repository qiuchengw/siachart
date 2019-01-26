#include "title.h"
#include "series.h"
#include "mainview.h"
#include "seriesname.h"
#include "panel.h"

namespace sia {

namespace charty {

Title::Title(Panel* parent)
    :PanelItem(SkString("title"), parent) {
    setZIndex(VisualItemZValue::kTitle);
}

void Title::preparePaints() {
    bk_ = declarePaint(SkString("background"));
    fg_ = declarePaint(SkString("text"));
    fg_->setAntiAlias(true);

    border_ = declarePaint(SkString("border"), SK_ColorRED);
    border_->setStyle(SkPaint::kStroke_Style);

    SkRect rc;
    if (getConfigGeometry(rc)) {
        setHeight(rc.height());
    }
}

void Title::onDraw(SkCanvas* canvas) {
    // 背景
    canvas->drawRect(rect(), *bk_);

    auto ctx = chartCtx();
    if (!ctx || flags_.test(Flags::kBlankTitle)) {
        return;
    }

    auto& cont_rc = contentRect();
    auto dict = dictionary();

    // 上边界
    SkScalar y = border_->getStrokeWidth() / 2;
    canvas->drawLine(0, y, width(), y, *border_);

    if (items_.empty()) {
        // 绘制symbol
        SkScalar x = contentRect().left() + 10, spaceing = 10;
        y = textVerticalMiddle(*fg_, height());
        SkString sym = ctx->symbol();
        canvas->drawString(ctx->symbol(), x, y, *fg_);
        x += fg_->measureText(sym.c_str(), sym.size()) + spaceing;

        // 绘制每个series的名字
        for (auto s : serieses()) {
            SkPaint tmp = *fg_;
            tmp.setColor(s->color());
            SkString dict_name = SeriesName::translateSeriesName(s->seriesName(), dict);
            canvas->drawString(dict_name, x, y, tmp);
            x += tmp.measureText(dict_name.c_str(), dict_name.size()) + spaceing;
        }
    } else {
        // 按照items设定绘制
        SkScalar x = contentRect().left() + 10, spacing = 10;
        y = textVerticalMiddle(*fg_, height());

        auto &prec = mainView()->precision();
        for (const PanelTitleItem& ti : items_) {
            SkPaint item_paint = *fg_;
            item_paint.setColor((Sk_InvalidColor == ti.color) ? fg_->getColor() : ti.color);

            SkString sitem;
            switch (ti.typ) {
            case PanelTitleItemType::kConstant:
                sitem = ti.item;
                break;

            case PanelTitleItemType::kChartSymbol:
                sitem = chartCtx()->symbol();
                break;

            case PanelTitleItemType::kSeriesName: {
                my_sp<MainView> view = nullptr;
                if (auto ss = series(ti.item, view)) {  // 在整个panel中查找序列
                    SkString dict_name = SeriesName::translateSeriesName(ss->seriesName(), dict);
                    chart_val_t val = ss->valAt(ctx->curVisualIndex());
                    sitem.appendf("%s:%s", dict_name.c_str(), view->precision().chartValFormat(val).c_str());
                    if (Sk_InvalidColor == ti.color) {
                        // 使用序列的颜色
                        item_paint.setColor(ss->color());
                    }
                }
                break;
            }
            }

            // val
            canvas->drawString(sitem, x, y, item_paint);
            x += item_paint.measureText(sitem.c_str(), sitem.size()) + spacing;
        }
    }
}

void Title::checkTitleItems() {
    for (const PanelTitleItem &t : items_) {
        if (t.typ == PanelTitleItemType::kSeriesName) {
            flags_.set(Flags::kDynamicItem, true);
            break;
        }
    }
}

void Title::addTitleItem(const PanelTitleItem& item) {
    items_.push_back(item);
    checkTitleItems();
}

void Title::removeTitleItem(const SkString& name) {
    auto i = std::remove_if(items_.begin(), items_.end(), [&](const PanelTitleItem& item)->bool {
        return item.item.equals(name);
    });

    if (i != items_.end()) {
        items_.pop_back_n(int(items_.end() - i));
    }
    checkTitleItems();
}

void Title::setItems(const SkTArray<PanelTitleItem>& items) {
    items_ = items;
    checkTitleItems();
}

void Title::setBlank(bool blank) {
    flags_.set(Flags::kBlankTitle, blank);
    if (!flags_.test(Flags::kBlankTitle)) {
        SkRect rc;
        if (getConfigGeometry(rc)) {
            setHeight(rc.height());
        }
    } else {
        setHeight(SkIntToScalar(10));
    }
}

void Title::seriesRemoved(const SkString& series_name) {
    removeTitleItem(series_name);
}

my_sp<Series> Title::series(const SkString& series_name, my_sp<MainView>& view) const {
    if (Panel* p = panel()) {
        return p->series(series_name, view);
    }
    return nullptr;
}

}

}

