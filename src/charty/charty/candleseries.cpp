#include "candleseries.h"
#include "SkPath.h"
#include "graphy/scene.h"
#include "mainview.h"

namespace sia {

namespace charty {

CandleSeries::CandleSeries(const SkString& series_name)
    :Series(SkString("candle_series"), series_name, SeriesType::kCandle) {
    setZIndex(VisualItemZValue::kCandleSeries);
}

void CandleSeries::onDraw(SkCanvas* canvas) {
    if (!mainView() || !ready() || !calcPoints()) {
        return;
    }

    const _DataPos& open = dataPosItem(SkString("open"));
    const _DataPos& high = dataPosItem(SkString("high"));
    const _DataPos& low = dataPosItem(SkString("low"));
    const _DataPos& close = dataPosItem(SkString("close"));

    int cnt = open.points.count();
    const SkScalar CANDLE_WIDTH = barWidth();
    const SkScalar OFFSET_X = CANDLE_WIDTH / 2;

    SkPath down_path, up_path;
    for (int i = 0; i < cnt; ++i) {
        if (!chartbase::validCoord(close.points[i].x())
                || !chartbase::validCoord(close.points[i].y())
                || !chartbase::validCoord(open.points[i].y())
                || !chartbase::validCoord(high.points[i].y())
                || !chartbase::validCoord(low.points[i].y())) {
            continue;
        }

        // 参考：https://en.wikipedia.org/wiki/Candlestick_chart
        // 如果 close > open, 则 unfill， 下边界： open，上边界close
        bool fill = (!(close.vals[i] > open.vals[i]));

        // 如果	close > prior_close，则 up（国内显示红色，和国外的习惯相反）
        bool up = false;
        if (i <= 0) {
            up = true;
        } else {
            up = (close.vals[i] > close.vals[i - 1]);
        }

        // 上边界是开，下边界是平
        SkRect rc;
        if (fill) {	// 高开低收
            rc = SkRect::MakeLTRB(open.points[i].x() - OFFSET_X, open.points[i].y(),
                                  close.points[i].x() + OFFSET_X, close.points[i].y());
        } else {	// 低开高收，与上面相反
            rc = SkRect::MakeLTRB(close.points[i].x() - OFFSET_X, close.points[i].y(),	// 上边界是close
                                  open.points[i].x() + OFFSET_X, open.points[i].y());	// 下边界是open
        }

        // draw rect的效率并不低，所以就直接画吧
        canvas->drawRect(rc, candleStickPaint(up, fill));

        // 头上的线是高（high）
        // canvas->drawLine(high.points[i].x(), high.points[i].y(), high.points[i].x(), rc.top(), paint);
        SkPath& path = up ? up_path : down_path;
        path.moveTo(high.points[i]);
        path.lineTo(high.points[i].x(), rc.top());

        // 脚下的线是低（low）
        // canvas->drawLine(low.points[i].x(), low.points[i].y(), low.points[i].x(), rc.bottom(), paint);
        path.moveTo(low.points[i]);
        path.lineTo(low.points[i].x(), rc.bottom());

        // sensor area
        setSensorArea(i, SkRect::MakeLTRB(rc.left(), high.points[i].y(), rc.right(), low.points[i].y()));
    }
    canvas->drawPath(down_path, candleStickPaint(false, false));
    canvas->drawPath(up_path, candleStickPaint(true, false));

    Series::onDraw(canvas);
}

void CandleSeries::preparePaints() {
    auto& ss = style();

    // wiki:
    // If the security closed higher than it opened, the body is hollow or unfilled,
    //	with the opening price at the bottom of the body and the closing price at the top.
    // If the security closed lower than it opened, the body is solid or filled,
    //	with the opening price at the top and the closing price at the bottom.

    // A black(or red) candle represents a price action with a lower closing price than the prior candle's close.
    //	A white (or green) candle represents a higher closing price than the prior candle's close.
    // Thus, the color of the candle represents the price movement relative to the prior period's close
    //	and the "fill" (solid or hollow) of the candle represents the price direction of the period
    //	in isolation (solid for a higher open and lower close; hollow for a lower open and a higher close).
    // A candlestick need not have either a body or a wick.[6]

    // 如果 close > open, 则 unfill， 下边界： open，上边界close
    // 如果	close > prior_close， 则红色（和国外的习惯相反）

    up_pat_.setColor(ss.up_color);
    up_pat_.setStrokeWidth(ss.stroke_width);
    // up_pat_.setStyle(ss.fill_bar ? SkPaint::kStrokeAndFill_Style : SkPaint::kStroke_Style);

    down_pat_ = up_pat_;
    down_pat_.setColor(ss.down_color);
}

SkPaint CandleSeries::candleStickPaint(bool up, bool fill) const {
    SkPaint ret = up ? up_pat_ : down_pat_;
    ret.setStyle(fill ? SkPaint::kFill_Style : SkPaint::kStroke_Style);
    return ret;
}

}

}

