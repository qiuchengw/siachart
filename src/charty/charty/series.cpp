#include "series.h"
#include "lineseries.h"
#include "mainview.h"
#include "chartctx.h"
#include "graphy/scene.h"
#include "barseries.h"
#include "candleseries.h"
#include "updownseries.h"
#include "panel.h"
#include "chart.h"
#include "seriesname.h"

namespace sia {

namespace charty {

Series::Series(const SkString& widget_name, const SkString& series_name, SeriesType typ)
    :ViewItem(ViewItemType::kViewSeries, widget_name, nullptr), series_name_(series_name), typ_(typ) {
    setTooltipCallback(nullptr);
}

Series::~Series() {
    if (isRemoveDatasource()) {
        auto *ctx = chartCtx();
        for (auto& dname : datanames()) {
            ctx->removeData(dname);
        }
    }
}

bool Series::setDataNames(const SkTArray<SkString>& datas) {
    datapos_.reset();
    for (const SkString& dn : datas) {
        _DataPos dp;
        dp.dataname = dn;
        datapos_.push_back(dp);
    }
    return true;
}

void Series::setDefaultDataName(const SkString& dataname) {
    default_dataname_ = dataname;
}

void Series::setRemoveDataOnDelete(bool on) {
    flags_.set(SeriesFlag::kRemoveDatasourceOnDelete, on);
}

bool Series::isRemoveDatasource() const {
    return flags_.test(SeriesFlag::kRemoveDatasourceOnDelete);
}

bool Series::calcPoints() {
    auto ctx = chartCtx();
    auto view = mainView();
    const auto &xpos = mainView()->indexXPos();
    if (!ctx || xpos.empty() || !view) {
        return false;
    }

    setIndexWidth(view->indexWidth());
    // 初始化坐标个数
    for (_DataPos& dp : datapos_) {
        dp.points.reset(xpos.count());
        dp.keys.reset(xpos.count());
        dp.vals.reset(xpos.count());
    }
    resetSensorAreas(xpos.count());

//     const SkScalar h = height();
//     const SkScalar dy = util::divide(h, SkScalar(view->maxY() - view->minY()));
    // 计算位置，并找到最大最小值的位置
    min_item_.val = std::numeric_limits<chart_val_t>::max();
    max_item_.val = std::numeric_limits<chart_val_t>::min();
    datasource()->forIndexData(ctx->indexBegin(), ctx->indexEnd(), [&](int32_t idx, const my_sp<chartbase::ColumnData> d)->bool {
        int32_t i = idx - ctx->indexBegin();
        for (_DataPos& dp : datapos_) {
            dp.vals[i] = d->get(dp.dataname);
            dp.keys[i] = d->key();
            if (chartbase::validVal(dp.vals[i])) {
                // y = h - (v - view->minY()) * dy;
                dp.points[i].set(xpos[i], view->yPosOfValue(dp.vals[i]));
                // 最大值
                if (dp.vals[i] > max_item_.val) {
                    max_item_.set(d->key(), dp.vals[i], dp.points[i]);
                }

                // 最小值
                if (dp.vals[i] < min_item_.val) {
                    min_item_.set(d->key(), dp.vals[i], dp.points[i]);
                }
            } else {
                dp.points[i].set(xpos[i], chartbase::kInvalidCoord);
            }
        }
        return true;
    });

    return true;
}

SkScalar Series::barWidth(SkScalar desire_width /*= SkIntToScalar(0)*/) {
    MainView* view = mainView();
    const SkScalar cont_height = view->contentHeight();
    const SkScalar MAX_INDEX_WIDTH = indexWidth();    // 每个bar最大可能的宽度
    SkScalar bar_width = desire_width;
    if (util::isZero(desire_width)) {
#define DEFAULT_BAR_SCALE SkScalar(0.5)
        bar_width = MAX_INDEX_WIDTH * DEFAULT_BAR_SCALE;
    } else {
        if (MAX_INDEX_WIDTH < bar_width) { // 不能比最大可能的宽度还要宽的！
            bar_width = MAX_INDEX_WIDTH;
        }
    }
    // 宽度取整，否则画出来的bar/candle可能是宽度不一
    bar_width = SkScalar(int(bar_width));
    return  (bar_width < 1.f) ? 1.f : bar_width;
}

std::set<SkString> Series::datanames() const {
    std::set<SkString> ret;
    for (const _DataPos& dp : datapos_) {
        ret.insert(dp.dataname);
    }
    return ret;
}

void Series::setColor(SkColor c) {
    style_.color = c;
    onStyleChanged();
}

void Series::setStyle(const SeriesStyle& ss) {
    style_ = ss;
    flags_.set(SeriesFlag::kUseCustomStyle, true);
    onStyleChanged();
}

const SeriesStyle& Series::style()const {
    return (flags_.test(SeriesFlag::kUseCustomStyle) ? style_ : siaSeriesStyle());
}

void Series::setTooltipCallback(SeriesTooltipCallback cb /*= nullptr*/) {
    // 如果设置为null，则设置一个默认的回调
    if (!cb) {
        auto dict = dictionary();
        tooltip_cb_ = [=](SeriesTooltipData& d)->bool {
            d.title = d.series_name;
            for (auto& i : d.datas) {
                SeriesTooltipData::Item item;
                SkString val = mainView()->precision().chartValFormat(i.second);
                item.cont.printf("%s: %s", dict->get(i.first).c_str(), val.c_str());
                item.color = Sk_InvalidColor;
                d.items.push_back(item);
            }
            return true;
        };
    } else {
        tooltip_cb_ = cb;
    }
}

void Series::showMinMaxTip(bool show) {
    flags_.set(SeriesFlag::kShowMinMaxTip, show);

    // 生成两个object？
}

chart_val_t Series::valAt(int visual_idx) const {
    if (visual_idx < 0
            || datapos_.empty()
            || visual_idx >= datapos_[0].itemCount()) {
        return chart_val_t(0);
    }

    if (default_dataname_.isEmpty()) {
        // 取第一个即可
        return datapos_[0].vals[visual_idx];
    }

    for (auto& t : datapos_) {
        if (default_dataname_.equals(t.dataname)) {
            return t.vals[visual_idx];
        }
    }
    return chart_val_t(0);
}

my_sp<Series> Series::create(const SkString& series_name, SeriesType typ) {
    my_sp<Series> series = nullptr;
    switch (typ) {
    case SeriesType::kBar:
        series = std::make_shared<BarSeries>(series_name);
        break;

    case SeriesType::kCandle:
        series = std::make_shared < CandleSeries>(series_name);
        break;

    case SeriesType::kLine:
        series = std::make_shared < LineSeries>(series_name);
        break;

    case SeriesType::kUpdownBar:
        series = std::make_shared < UpdownSeries>(series_name);
        break;
    }
    return series;
}

void Series::onSensed(int idx, const SkPoint& pt) {
    auto ctx = chartCtx();
    if (!ctx->functionEnabled(Chart::kFunctionTooltip)) {
        return;
    }

    auto dict = dictionary();
    // 提示tooltip
    if (auto scene = dynamic_cast<graphy::Scene*>(root())) {
        ViewItemTooltip* tip = mainView()->seriesTip();
        if (!tooltip_cb_ || !tip) {
            return;
        }

        // 组装tooltip数据
        SeriesTooltipData tip_data;
        tip_data.setPanel(HPanel(panel()));
        tip_data.series_name = SeriesName::translateSeriesName(series_name_, dict);
        tip_data.typ = typ_;
        for (auto& dp : datapos_) {
            tip_data.key = dp.keys[idx];
            SkString dname = SeriesName::translateDataName(dp.dataname, dict);
            tip_data.datas.insert({ dname, dp.vals[idx] });
        }

        // 显示tooltip
        if (tooltip_cb_(tip_data)) {
            SkPoint pt_win = clientToWindow(pt);
            tip->moveTo(pt_win.x(), pt_win.y());

            tip->setTitle(tip_data.title);
            tip->setBodyItems(tip_data.items);
            scene->requestTooltip(tip);
        }
    }
}

const Series::_DataPos& Series::dataPosItem(const SkString& dataname) const {
    static const _DataPos _INVALID_DATA;
    for (auto& dp : datapos_) {
        if (dataname.equals(dp.dataname)) {
            return dp;
        }
    }
    return _INVALID_DATA;
}

/*! \brief 调整target在cont中的位置，确保target能尽量完全显示
 *
 *    @param	pt	target 指向的点
 *    @param	target	显示的矩形
 *    @param	cont	容器矩形
 *    @return
 */
SkRect adjustRect(const SkPoint& pt, const SkSize& target, const SkRect& cont, SkPoint& pt_start) {
    SkRect rc = SkRect::MakeSize(target);
    const SkScalar SPACING = SkIntToScalar(15);
    const SkScalar LINE_SPACING = SkIntToScalar(4);
    bool pos_left = false;
    if (pt.x() > cont.centerX()) {
        // 显示在左边
        rc.offsetTo(pt.x() - SPACING - target.width() - LINE_SPACING, pt.y() - target.height() / 2 );
        pos_left = true;
    } else {
        // 显示在右边
        rc.offsetTo(pt.x() + SPACING + LINE_SPACING, pt.y() - target.height() / 2);
    }

    // 调整y
    if (rc.left() < cont.left()) {
        rc.offsetTo(cont.left(), rc.top());
    }

    if (rc.top() < cont.top()) {
        rc.offsetTo(rc.left(), cont.top());
    }

    if (rc.right() > cont.right()) {
        rc.offset(cont.right() - rc.right(), 0);
    }

    if (rc.bottom() > cont.bottom()) {
        rc.offset(0, cont.bottom() - rc.bottom());
    }

    if (pos_left) {
        pt_start.set(rc.right() + LINE_SPACING, rc.centerY());
    } else {
        pt_start.set(rc.left() - LINE_SPACING, rc.centerY());
    }
    return rc;
}

void Series::onDraw(SkCanvas* canvas) {
    if (flags_.test(SeriesFlag::kShowMinMaxTip)) {
        auto& prec = mainView()->precision();
        SkPaint paint;
        paint.setColor(SK_ColorCYAN);
        // paint.setStyle(SkPaint::kStroke_Style);
        // paint.setAntiAlias(true);

        SkPoint pt_start;
        SkString str = prec.chartValFormat(min_item_.val);
        SkRect rc = textRect(str, paint);
        rc = adjustRect(min_item_.point, SkSize::Make(rc.width(), rc.height()), rect(), pt_start);

        // 画最小值
        // canvas->drawRect(rc, paint);
        canvas->drawString(str, rc.left(), rc.bottom(), paint);
        canvas->drawLine(pt_start, min_item_.point, paint);

        // 画最大值
        str = prec.chartValFormat(max_item_.val);
        rc = textRect(str, paint);
        rc = adjustRect(max_item_.point, SkSize::Make(rc.width(), rc.height()), rect(), pt_start);
        // canvas->drawRect(rc, paint);
        canvas->drawString(str, rc.left(), rc.bottom(), paint);
        canvas->drawLine(pt_start, max_item_.point, paint);
    }
    ViewItem::onDraw(canvas);
}

void Series::onStyleChanged() {
    preparePaints();
}

}

}

