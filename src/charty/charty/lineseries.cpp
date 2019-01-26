#include "lineseries.h"
#include "SkPath.h"
#include "graphy/scene.h"
#include "mainview.h"

namespace sia {

namespace charty {

LineSeries::LineSeries(const SkString& series_name)
    :Series(SkString("line_series"), series_name, SeriesType::kLine) {
    setZIndex(VisualItemZValue::kLineSeries);
}

void LineSeries::onDraw(SkCanvas* canvas) {
    if (!mainView() || !ready() || !calcPoints()) {
        return;
    }

    pat_.setColor(color());
    SkPath path;
    for (auto& dp : datapos_) {
        const auto& pts = dp.points;
        if (pts.empty()) {
            continue;
        }

        // 我们 要把有效的位置都给记录下来
        for (int i = 0; i < pts.count(); ++i) {
            // 找到有效的点
            if (!chartbase::validCoord(pts[i])) {
                continue;
            }

            // 记录感应区
            // 以pt为中心点，在x轴上左右等分扩展到indexWidth大小。
            // 在 y 轴上等分扩展到 2 个像素
            setSensorArea(i, pts[i], indexWidth(), 4);

            if (i > 0 && chartbase::validCoord(pts[i - 1])) {
                path.lineTo(pts[i]);
            } else {
                path.moveTo(pts[i]);
            }
        }
    }
    canvas->drawPath(path, pat_);

    Series::onDraw(canvas);
}

void LineSeries::preparePaints() {
    auto& ss = style();

    pat_.setStyle(SkPaint::kStroke_Style);
    pat_.setColor(color());
    pat_.setStrokeWidth(ss.stroke_width);
    pat_.setStrokeCap(SkPaint::kRound_Cap);
    pat_.setStrokeJoin(SkPaint::kDefault_Join);
    pat_.setAntiAlias(ss.antialias);
}

}

}

