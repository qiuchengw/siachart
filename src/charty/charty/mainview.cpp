#include "mainview.h"
#include <cassert>
#include "series.h"
#include "objectitem.h"

namespace sia {
namespace charty {

MainView::MainView(Panel* parent)
    :PanelItem(SkString("mainview"), parent) {
    setZIndex(VisualItemZValue::kBackground);
    setLayout(&stacklay_);
}

MainView::~MainView() {
    // 删除series
}

void MainView::setAsSubview(bool on) {
    // subview 在创建的时候指定，并且不能被更改为主View
    // 副图没有背景色
    flags_.set(Flags::kSubView, on);
}

my_sp<Series> MainView::addSeries(const SkString& name, SeriesType typ, const SkTArray<SkString>& datas) {
    if (series(name)) {
        assert(false);
        return nullptr;
    }

    if (auto series = Series::create(name, typ)) {
        if (series->setDataNames(datas)) {
            series->setChartContext(chartCtx());
            series_.insert({ name, series });
            addChild(series.get());
            return series;
        }
    }
    return nullptr;
}

bool MainView::removeSeries(const SkString& name) {
    if (auto ss = series(name)) {
        // 通知到图表，即将删除序列
        SkEvent evt(ChartEventType::kSeriesBeforeDelete);
        evt.setString("name", name.c_str());
        chartCtx()->emitEvent(evt);

        // 检查有哪些object是依赖于这个series的，也要一并删掉
        bool has_related = false;
        do {
            has_related = false;
            for (auto i : objects_) {
                if (i.second->isAttachedTo(ss.get())) {
                    removeObject(i.first);
                    has_related = true;
                    break;
                }
            }
        } while (has_related);

        // ss->setParent(nullptr);
        removeChild(ss.get());
        series_.erase(name);

        // 重新计算图表的显示
        recalc(0);

        // 序列已删除!
        // ！！！注意：此消息执行后，可能导致this指针实例被删除，
        // ！！！所以此消息之后不可以再执行任何代码！
        SkEvent evt_del(ChartEventType::kSeriesDeleted);
        evt_del.setString("name", name.c_str());
        chartCtx()->emitEvent(evt_del);

        return true;
    }
    return false;
}

bool MainView::addObject(const SkString& name, const ObjectParam&para, my_sp<IVisualObject> obj) {
    auto item = std::make_shared<ObjectItem>(name, this);
    objects_[name] = item;

    item->setChartContext(chartCtx());
    item->setParam(para, obj);
    addChild(item.get());

    return true;
}

bool MainView::removeObject(const SkString& name) {
    auto i = objects_.find(name);
    if (i == objects_.end()) {
        return false;
    }

    auto item = i->second;
    removeChild(item.get());
    objects_.erase(i);

    return true;
}

ViewItem* MainView::viewItem(const SkString& name) const {
    // 因为series同时使用了seriesname 和 widget_name,所以不能用下面这种方式遍历了
    // 比如 Candle 是name() == "candle_series"。 它的series_name 才是我们真正感兴趣的name
    // ViewItem* ret = nullptr;
    //     forEachChild([&](Widget* w)->bool {
//         if (w->name().equals(name)) {
//             ret = dynamic_cast<ViewItem*>(w);
//             return false;
//         }
//         return true;
//     });

    // 遍历series
    for (auto i : series_) {
        if (name.equals(i.second->seriesName())) {
            return i.second.get();
        }
    }

    // 遍历object
    for (auto i : objects_) {
        if (name.equals(i.second->name())) {
            return i.second.get();
        }
    }
    return nullptr;
}

my_sp<Series> MainView::series(const SkString& name)const {
    auto i = series_.find(name);
    if (i != series_.end()) {
        return i->second;
    }
    return nullptr;
}

std::vector<my_sp<Series>> MainView::serieses() const {
    std::vector<my_sp<Series>> ret;
    for (auto i : series_) {
        ret.push_back(i.second);
    }
    return ret;
}

void MainView::recalc(Bits flags) {
    calcXPoss();
    calcYRange();
}

void MainView::setValueSlice(int count) {
    vslice_.setSlice(count);
}

void MainView::setSeriesColor(const std::map<SkString, SkColor>& colors) {
    for (auto i : colors) {
        if (auto s = series(i.first)) {
            s->setColor(i.second);
        }
    }
}

void MainView::setSeriesStyle(const std::map<SkString, SeriesStyle>& styles) {
    for (auto i : styles) {
        if (auto s = series(i.first)) {
            s->setStyle(i.second);
        }
    }
}

void MainView::setSeriesTooltipCallback(std::map<SkString, SeriesTooltipCallback> callbacks) {
    for (auto i : callbacks) {
        if (auto s = series(i.first)) {
            s->setTooltipCallback(i.second);
        }
    }
}

void MainView::setMiddleVal(chart_val_t val) {
    vslice_.setMiddleVal(val);
    flags_.set(Flags::kMiddleVal, chartbase::validVal(val));
}

void MainView::showSeriesMinMaxTip(const SkString&series_name, bool show) {
    if (auto s = series(series_name)) {
        s->showMinMaxTip(show);
    }
}

int MainView::dataIndexOfPos(SkScalar x) const {
    int ret = visualIndexOfPos(x);
    if (ret >= 0) {
        if (auto ctx = chartCtx()) {
            ret += ctx->indexBegin();
            if (ret < ctx->totalIndexCount()) {
                return ret;
            }
        }
    }
    return -1;
}

int MainView::visualIndexOfPos(SkScalar x) const {
    if (xpos_.empty()) {
        return -1;
    }

    if (x < 0) {
        return 0;
    }

    int ret = (int)(x * xpos_.count() / contentWidth());
    if (ret >= xpos_.count()) {
        return xpos_.count() - 1;
    }
    return ret;
}

int MainView::visualIndexOfKey(chart_key_t key) const {
    auto ctx = chartCtx();
    auto ds = datasource();

    chart_key_t k1 = ds->key(ctx->indexBegin());
    if (key < k1) {
        return -1;
    }

    chart_key_t k2 = ds->key(ctx->indexEnd());
    if (key > k2) {
        return -1;
    }

    // 得通过数据源找到其index
    return ds->keyIndex(key, ctx->indexBegin(), ctx->indexEnd()) - ctx->indexBegin();
}

double MainView::valueOfYPos(SkScalar y) const {
    return vslice_.valueOfYPos(y, contentHeight());
//     const double v_per_dpi = util::divide((double)(maxY() - minY()), (double)contentHeight());
//     return maxY() - y * v_per_dpi;
}

SkScalar MainView::yPosOfValue(chart_val_t v) const {
    return vslice_.yPosOfValue(v, contentHeight());
//     const double h = contentHeight();
//     // 每1值占用的高度
//     const double dy = util::divide(h, (double)(maxY() - minY()));
//     return (SkScalar)(h - (v - minY()) * dy);
}

SkPoint MainView::posOfDataname(int idx, const SkString& dataname) const {
    chart_val_t v = datasource()->value(idx, dataname);
    if (chartbase::kInvalidDataValue == v) {
        return SkPoint::Make(0, 0);
    }

    idx -= (chartCtx()->indexBegin());
    if (idx < 0 || idx >= xpos_.count()) {
        return SkPoint::Make(0, 0);
    }
    return SkPoint::Make(xpos_[idx], yPosOfValue(v));
}

SkScalar MainView::xPosOfVisualIndex(int visual_idx) const {
    if (visual_idx < 0 || visual_idx >= xpos_.count()) {
        return SkIntToScalar(-1);
    }
    return xpos_[visual_idx];
}

SkScalar MainView::xPosOfKey(chart_key_t key) const {
    ChartContext* ctx = chartCtx();
    auto idx_begin = ctx->indexBegin();
    auto idx = datasource()->keyIndex(key, idx_begin, ctx->indexEnd()) - idx_begin;
    return xPosOfVisualIndex(idx);
}

chart_key_t MainView::datetimeOfIndex(int idx) const {
    return datasource()->key(idx);
}

bool MainView::viewEmpty() const {
    return series_.empty() && objects_.empty();
}

SkScalar MainView::indexWidth() const {
    if (auto ctx = chartCtx()) {
        auto count = ctx->curIndexCount();
        if (count > 0) {
            // 一个index的宽度
            return contentWidth() / count;
        }
    }
    return SkIntToScalar(0);
}

void MainView::onDraw(SkCanvas* canvas) {
    // 画背景
    if (!flags_.test(Flags::kSubView)) {
        // 副图是没有背景色的
        const auto rc = rect();
        canvas->drawRect(rect(), *bk_);
        grid_.draw(canvas, this, width(), contentRect().height(), vslice_.valPos());
    }
    VisualItem::onDraw(canvas);
}

bool MainView::onMouse(int x, int y, sia::InputState state, uint32_t modifiers) {
    // 因为在这里面的孩子节点，都是完全相互覆盖的。
    // 按照正常的处理流程，只有最上层的一个孩子能够得到鼠标消息
    // 所以我们直接接管消息。然后依此传递到孩子节点中
    bool handled = false;
    // object 在最上层，先处理
    for (auto i : objects_) {
        handled = i.second->onMouse(x, y, state, modifiers);
        if (handled) {
            break;
        }
    }

    if (!handled) {
        for (auto i : series_) {
            handled = i.second->onMouse(x, y, state, modifiers);
            if (handled) {
                break;
            }
        }
    }
    return handled;
}

void MainView::onSizeChange() {
    calcXPoss();
    calcYRange();
    vslice_.calc(precision(), contentHeight());

    VisualItem::onSizeChange();
}

void MainView::preparePaints() {
    tip_.preparePaints();

    bk_ = declarePaint(SkString("background"));
    grid_.setPaints(declarePaint(SkString("grid")));
}

void MainView::calcXPoss() {
    xpos_.reset();

    if (auto ds = datasource()) {
        auto count = chartCtx()->curIndexCount();
        if (count > 0) {
            // 一个index的宽度
            const double w_one = indexWidth()/*contentWidth() / count*/;
            // 我们取到中心点
            double x = w_one / 2 + contentX();
            for (int i = 0; i < count; ++i) {
                xpos_.push_back((SkScalar)x);
                x += w_one;
            }
        }
    }
}

void MainView::calcYRange() {
    if (!ready()) {
        return;
    }

    if (series_.empty()) {
        // 算档位信息
        vslice_.setMaxMin(chartbase::kInvalidDataValue, chartbase::kInvalidDataValue);
        vslice_.calc(precision(), contentHeight());
    } else {
        // 获取当前显示的序列最大和最小值
        auto ctx = chartCtx();
        chart_val_t min_y, max_y;
        if (datasource()->getIndexMinMaxVal(datanames(), ctx->indexBegin(), ctx->indexEnd(), min_y, max_y)) {
            // 算档位信息
            vslice_.setMaxMin(min_y, max_y);
            vslice_.calc(precision(), contentHeight());
        }
    }

}

std::set<SkString> MainView::datanames() const {
    std::set<SkString> ret;
    for (auto i : series_) {
        for (auto& name : i.second->datanames()) {
            ret.insert(name);
        }
    }
    return ret;
}

}

}

