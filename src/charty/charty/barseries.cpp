#include "barseries.h"
#include "SkPath.h"
#include "graphy/scene.h"
#include "mainview.h"

namespace sia {

namespace charty {

BarSeries::BarSeries(const SkString& series_name)
    :Series(SkString("bar_series"), series_name, SeriesType::kBar) {
    setZIndex(VisualItemZValue::kBarSeries);
}

void BarSeries::onDraw(SkCanvas* canvas) {
    if (!mainView() || !ready() || !calcPoints()) {
        return;
    }

    pat_.setColor(color());

    const SkScalar cont_height = contentHeight();
    const SkScalar bar_width = barWidth(style().bar_width);
    const SkScalar offset_x = bar_width / 2;
    for (auto& dp : datapos_) {
        auto& pts = dp.points;
        int idx = 0;
        for (const SkPoint& pt : pts) {
            if (!isValidPoint(pt)) {
                idx++;
                continue;
            }

            // »­Ò»¸ö¾ØÐÎ
            SkRect rc = SkRect::MakeXYWH(pt.x() - offset_x, pt.y(), bar_width, cont_height - pt.y());
            canvas->drawRect(rc, pat_);
            setSensorArea(idx, rc);
            idx++;
        }
    }
    Series::onDraw(canvas);
}

void BarSeries::preparePaints() {
    auto& ss = style();

    pat_.setColor(color());
    pat_.setStrokeWidth(ss.stroke_width);

    if (ss.bar_width > 1) {
        pat_.setStyle(ss.fill_bar ? SkPaint::kFill_Style : SkPaint::kStroke_Style);
    } else {
        pat_.setStyle(SkPaint::kFill_Style);
    }
}

}

}

