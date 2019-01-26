#include "chart.h"
#include <set>

#include "chartlayout.h"
#include "panel.h"
#include "crosshair.h"
#include "viewitem.h"
#include "mousestate.h"
#include "series.h"
#include "chartman.h"
#include "private.h"

namespace sia {
namespace charty {


bool ChartPrivate::ViewCache::snapshot(SkCanvas* canvas) {
    if (surf) {
        snap = surf->makeImageSnapshot();
        return !!snap;
    }
    return false;
}

bool ChartPrivate::ViewCache::draw(SkCanvas* canvas) {
    if (hasSnap() && canvas) {
        canvas->drawImage(snap, 0, 0);
        return true;
    }
    return false;
}

Chart::Chart(const SkString& id)
    :ChartBase(id) {
    ChartMan::instance()->regInst(this);

    setVisibleP(true);

    d.reset(new ChartPrivate());
    // 默认情况下，启用所有的内建功能
    d->flags.set(Chart::FunctionType::kFunctionAll, true);

    d->ctx.setChart(this);
    setLayout(&(d->layout));
    d->layout.setPanels(&(d->panels));

    d->cross.setChartContext(&(d->ctx));
}

Chart::~Chart() {
    ChartMan::instance()->unregInst(this);
}

SkString Chart::ID() const {
    return name();
}

HPANEL Chart::addPanel(/*bool with_calendar*/) {
    // 新的panel总是添加到最后一个，带有calendar
    // 先不要传入parent参数的，这种方式不能正确的进行布局应用
    Panel* p = new Panel(nullptr);
    p->setChartContext(&(d->ctx));

    if (!d->panels.empty()) {
        if (Panel* last_panel = d->panels.back()) {
            // 把目前最后一个panel的calendar去掉
            last_panel->showItem(PanelItemType::kPanelCalendar, false);

            p->showItem(PanelItemType::kPanelRatioAxis, last_panel->hasItem(PanelItemType::kPanelRatioAxis));
            p->showItem(PanelItemType::kPanelValueAxis, last_panel->hasItem(PanelItemType::kPanelValueAxis));
        }
    }
    d->panels.push_back(p);
    addChild(p);

    update(0);
    return HPanel(p);
}

uint32_t Chart::panelCount() const {
    return (uint32_t)(d->panels.size());
}

HPANEL Chart::panel(uint32_t idx) const {
    if (idx >= panelCount()) {
        return nullptr;
    }
    return HPanel(d->panels[idx]);
}

bool Chart::removePanel(HPANEL panel) {
    if (Panel* p = ToPanel(panel)) {
        Panel* last_panel = nullptr;
        if (!d->panels.empty()) {
            last_panel = d->panels.back();
        }
        bool found = false;
        for (auto i = d->panels.begin(); i != d->panels.end(); ++i) {
            if (*i == p) {
                // 从chart控件中移除
                removeChild(p);
                // 从记录中移除
                d->panels.erase(i);
                delete p;
                found = true;
                break;
            }
        }

        // 确保最后一个panel带有calendar
        if (found && (last_panel == p) && !d->panels.empty()) {
            last_panel = d->panels.back();
            last_panel->showItem(PanelItemType::kPanelCalendar, true);
        }

        if (found || d->panels.empty()) {
            d->selected = nullptr;
            // 删除面板一定要全面的更新界面
            update(0);
        }
        return found;
    }
    return false;
}

bool Chart::addSeries(HPANEL panel, const SkString& name, SeriesType typ, const SkTArray<SkString>& datas) {
    if (Panel* p = ToPanel(panel)) {
        return p->mainView()->addSeries(name, typ, datas) != nullptr;
    }
    return false;
}

bool Chart::addSeries(HPANEL panel, const SkString& series_name, SeriesType typ, const SkString& dataname) {
    SkTArray<SkString> datas;
    datas.push_back(dataname);
    return addSeries(panel, series_name, typ, datas);
}

bool Chart::getSeries(HPANEL panel, SkTArray<SkString>& series_names) {
    if (Panel* p = ToPanel(panel)) {
        for (auto s : p->mainView()->serieses()) {
            series_names.push_back(s->seriesName());
        }
        return true;
    }
    return false;
}

bool Chart::selectSeries(HPANEL panel, const SkString& series_name) {
    if (nullptr == panel) {
        if (d->selected) {
            d->selected->select(false);
            return true;
        }
        return false;
    }

    if (!functionEnabled(Chart::kFunctionSelectSeries)) {
        return false;
    }

    if (Panel* p = ToPanel(panel)) {
        if (auto s = p->mainView()->series(series_name)) {
            s->select(true);
            return true;
        }
    }
    return false;
}

void Chart::showSeriesMinMaxTip(HPANEL panel, const SkString&series_name, bool show) {
    if (Panel* p = ToPanel(panel)) {
        p->mainView()->showSeriesMinMaxTip(series_name, show);
    }
}

void Chart::removeSeries(HPANEL panel, const SkString& series_name) {
    if (Panel* p = ToPanel(panel)) {
        p->mainView()->removeSeries(series_name);
        d->selected = nullptr;
        update(0);
    }
}

void Chart::setSeriesColor(HPANEL panel, const std::map<SkString, SkColor>& colors) {
    if (Panel* p = ToPanel(panel)) {
        p->mainView()->setSeriesColor(colors);
    }
}

void Chart::setSeriesStyle(HPANEL panel, const std::map<SkString, SeriesStyle>& styles) {
    if (Panel* p = ToPanel(panel)) {
        p->mainView()->setSeriesStyle(styles);
    }
}

void Chart::setMiddleVal(HPANEL panel, chart_val_t val) {
    if (Panel* p = ToPanel(panel)) {
        p->mainView()->setMiddleVal(val);
        update(0);
    }
}

void Chart::addData(const SkString& data_name, const std::map<chart_key_t, chart_val_t>& datas) {
    d->ctx.addData(data_name, datas);
}

void Chart::removeData(const SkString& data_name) {
    d->ctx.removeData(data_name);
}

void Chart::removeData(const SkString& data_name, const SkTArray<chart_key_t>& keys) {
    d->ctx.removeData(data_name, keys);
}

void Chart::setCurrentDomain(int32_t begin, int32_t end) {
    d->ctx.setCurrentDomain(begin, end);
}

void Chart::setSymbol(const SkString& symbol) {
    d->ctx.setSymbol(symbol);
}

bool Chart::setTitleItems(HPANEL panel, const SkTArray<PanelTitleItem>& items) {
    if (Panel* p = ToPanel(panel)) {
        if (Title *t = p->title()) {
            t->setItems(items);
        }
        return true;
    }
    return false;
}

void Chart::showPanelTitle(HPANEL panel, bool show) {
    if (Panel* p = ToPanel(panel)) {
        p->showItem(PanelItemType::kPanelTitle, show);
    }
}

void Chart::showPanelItem(int item, bool show) {
//     if (item != PanelItemType::kPanelRatioAxis
//         && item != PanelItemType::kPanelValueAxis) {
//         return;
//     }

    for (Panel* p : d->panels) {
        p->showItem(item, show);
    }
}

void Chart::setValueSlice(HPANEL panel, int count) {
    if (Panel* p = ToPanel(panel)) {
        p->mainView()->setValueSlice(count);
        update(0);
    }
}

void Chart::zoomIn() {
    ChartContext& ctx = d->ctx;
    int level = 0;
    if (ctx.curIndexCount() / 3 * 2 > 5) {
        level = ctx.curIndexCount() / 3;
        setCurrentDomain(ctx.indexBegin() + level, ctx.indexEnd());
    }
}

void Chart::zoomOut() {
    ChartContext& ctx = d->ctx;
    auto cur_view_cout = ctx.curIndexCount();
    // 我们每次在当前的显示个数上扩大1倍（所以就是指数级别缩放1,2,4,8）
    if (ctx.indexBegin() > 0) {
        // 向左扩展
        ctx.setCurrentDomain(ctx.indexBegin() - cur_view_cout, ctx.indexEnd());
    } else {
        // 向右扩展
        ctx.setCurrentDomain(0, ctx.indexEnd() + cur_view_cout);
    }
}

void Chart::setCrossHairDataname(const SkString& datanme) {
    d->cross.attachDataname(datanme);
}

void Chart::setCrossHairDetail(CrosshairDetail* detail) {
    d->cross.setDetail(detail);
}

void Chart::setSeriesTooltipCallback(HPANEL panel, std::map<SkString, SeriesTooltipCallback> callbacks) {
    if (Panel* p = ToPanel(panel)) {
        p->mainView()->setSeriesTooltipCallback(callbacks);
    }
}

int32_t Chart::showFormula(HPANEL panel, const SkString& ta_name, const formula::TaParam& param) {
    if (nullptr == panel) {
        panel = addPanel();
    }

    return ToPanel(panel)->showFormula(ta_name, param);
}

void Chart::removeFormula(HPANEL panel, int32_t formula_id) {
    if (Panel* p = ToPanel(panel)) {
        p->removeFormula(formula_id);
    }
}

bool Chart::addObject(HPANEL panel, const SkString& name, const ObjectParam&para, my_sp<IVisualObject> obj) {
    Panel* p = ToPanel(panel);
    if (!p || !obj) {
        return false;
    }
    return p->mainView()->addObject(name, para, obj);
}

void Chart::removeObject(HPANEL panel, const SkString& object_name) {
    if (Panel* p = ToPanel(panel)) {
        p->mainView()->removeObject(object_name);
    }
}

void Chart::setCalendarFormat(const SkString& format, const SkString& crosshair_detail_format) {
    d->ctx.setCalendarFormat(format, crosshair_detail_format);
}

void Chart::moveCrosshair(int offset) {
    if (!d->cross.flag(CrossHair::kHasCross)) { // 没有显示cross
        d->cross.show(CrossHair::kHasCross | CrossHair::kHasRubber);

        // 设置主panel为当前的panel
        if (!d->panels.empty()) {
            Panel* main_p = d->panels.front();
            d->cross.setPanel(main_p, main_p);
        }
    }

    // 移动
    d->cross.move(offset);
}

bool Chart::moveView(int offset) {
    if (0 == offset) {
        return false;
    }

    int last_idx = d->ctx.totalIndexCount() - 1;
    int idx_begin = d->ctx.indexBegin();
    int idx_end = d->ctx.indexEnd();

    if ((offset > 0 && idx_end >= last_idx)	// 已经最右，不能右移
            || (offset < 0 && idx_begin <= 0)) {	// 已经最左，不能左移
        return false;
    }

    idx_begin += offset;
    idx_end += offset;

    if (idx_begin < 0) {
        idx_begin = 0;
    }

    if (idx_end > d->ctx.totalIndexCount()) {
        idx_end = d->ctx.totalIndexCount() - 1;
    }

    setCurrentDomain(idx_begin, idx_end);

    return true;
}

void Chart::setPrecision(HPANEL panel, int16_t precision, int32_t divisor) {
    if (Panel* p = ToPanel(panel)) {
        p->mainView()->precision().setPrecision(precision, divisor);
    }
}

void Chart::setFunction(int32_t functions, bool enable) {
    d->flags.set(functions, enable);

// 	if (functionEnabled(Chart::kFunctionSelectSeries) && d->selected) {
// 		selectSeries(nullptr, SkString(""));
// 	}
}

bool Chart::functionEnabled(int32_t functions) const {
    return d->flags.test(functions);
}

void Chart::showGrid(HPANEL panel, bool horizontal, bool vertical) {
    if (Panel* p = ToPanel(panel)) {
        p->mainView()->grid().showGrid(horizontal, vertical);
    }
}

void Chart::setGridVerticalSlice(HPANEL panel, int line_count) {
    if (Panel* p = ToPanel(panel)) {
        p->mainView()->grid().setGridVerticalSlice(line_count);
    }
}

void Chart::setGridVerticalSlice(HPANEL panel, const SkTArray<int>& visual_indexes, int middle_index) {
    if (Panel* p = ToPanel(panel)) {
        p->mainView()->grid().setGridVerticalSlice(visual_indexes, middle_index);
    }
}

void Chart::setGridVerticalSlice(HPANEL panel, const SkTArray<chart_key_t>& keys_slice, chart_key_t middle_key /*= kInvalidDataKey*/) {
    if (Panel* p = ToPanel(panel)) {
        p->mainView()->grid().setGridVerticalSlice(keys_slice, middle_key);
    }
}

bool Chart::mergePanel(HPANEL main, HPANEL sub) {
    Panel* p_main = ToPanel(main);
    Panel* p_sub = ToPanel(sub);

    if (!p_main || !p_sub) {
        return false;
    }

    if (p_main->mergePanel(p_sub)) {
        return removePanel(sub);
    }
    return false;
}

void Chart::onDraw(SkCanvas* canvas) {
    static int _i = 0;
    if (d->flags.testReset(ChartPrivate::kDataDirty)) {
        update(ChartPrivate::kDataDirty);
    }

    // 只有视图确实脏掉了，才会去全面的绘制，否则只是使用之前的cache绘制即可
    if (d->flags.test(ChartPrivate::kViewDirty)) {
        ChartBase::onDraw(canvas);
        // 拍照
        d->vcache.snapshot(canvas);
        std::cout << "Chart redraw" << _i++ << std::endl;
    } else {
        // 使用cache绘制
        d->vcache.draw(canvas);
        std::cout << "Chart use cache draw" << _i++ << std::endl;
    }

    if (!d->panels.empty()) {
        // 最后才画十字线，只有当有面板的时候才会有十字线。
        if (d->cross.showing()) {
            d->cross.draw(canvas);
        }
    } else {
        canvas->clear(SK_ColorTRANSPARENT);
    }

    // 重绘后，界面不再脏了！
    d->flags.set(ChartPrivate::kViewDirty, false);
}

bool Chart::onEvent(const SkEvent& evt) {
    bool handled = false;
    if (evt.isType(ChartEventType::kDataChanged)
            || evt.isType(ChartEventType::kDomainChanged)) {
        d->flags.set(ChartPrivate::kDataDirty | ChartPrivate::kViewDirty, true);
        handled = true;
    } else if (evt.isType(ChartEventType::kViewItemSeletChanged)) {
        ViewItem* item = nullptr;
        evt.findPtr("item", (void**)(&item));
        if (item != d->selected) {
            if (d->selected) {
                d->selected->select(false);
            }

            // 要重绘
            d->selected = item;
            redraw();
            handled = true;
        }
    } else if (evt.isType(ChartEventType::kViewItemDeleted)) {
        ViewItem* item = nullptr;
        evt.findPtr("item", (void**)(&item));
        if (item == d->selected) {
            d->selected = nullptr;
        }
        redraw();
        handled = true;
    } else if (evt.isType(ChartEventType::kSiaSeriesStyleChanged)
               || evt.isType(ChartEventType::kSiaThemeChanged)) {
        // 广播的事件导入到到cross中
        d->cross.broadcast(evt);
        redraw();
        handled = true;
    } else if (evt.isType(ChartEventType::kSeriesDeleted)) {
        SkString series_name(evt.findString("name"));
        for (Panel* p : d->panels) {
            p->seriesRemoved(series_name);
        }
        redraw();
        handled = true;
    }
    return handled;
}

void Chart::update(Bits flags) {
    // 更新前先隐藏tooltip
    hideTooltip();

    for (Panel* p : d->panels) {
        p->recalc(flags);
    }
    redraw();
}

void Chart::redraw() {
    d->flags.set(ChartPrivate::kViewDirty, true);
    relayout();
}

bool Chart::onMouse(int x, int y, sia::InputState state, uint32_t modifiers) {
    d->mouse_state.onMouse(x, y, state, modifiers);
    if (d->panels.empty()
            || !d->flags.test(Chart::kFunctionMouseInput)) {
        return false;
    }

    // 鼠标事件先去掉tooltip
    hideTooltip();

    switch (state) {
    case InputState::kDoubleClick_InputState:
        if (d->flags.test(Chart::kFunctionCrossHair)) {
            if (d->cross.showing()) {
                d->cross.hide();
            } else {
                auto flag = d->flags.test(Chart::kFunctionRubberSelect) ? CrossHair::kHasRubber : 0;
                d->cross.show(CrossHair::kHasCross | flag);
            }
        }
        break;

    case InputState::kMove_InputState:
        if (d->flags.test(Chart::kFunctionRubberSelect)) {
            if (d->mouse_state.dragging()) {
                d->cross.show(CrossHair::kHasRubber);
            }
        }
        break;
    }

    bool handled = false;
    if (d->cross.showing()) {
        // 设置当前的Panel
        for (auto p : d->panels) {
            if (p->geometry().contains(SkIntToScalar(x), SkIntToScalar(y))) {
                d->cross.setPanel(*(d->panels.begin()), p);
                break;
            }
        }
        handled = d->cross.onMouse(x, y, state, modifiers);
    } else {
        if (InputState::kDown_InputState == state
                || InputState::kUp_InputState == state) {
            // Down/up消息一定要传进去，但是并不需要他的处理结果
            d->cross.onMouse(x, y, state, modifiers);
        }
    }

    if (!handled) {
        return ChartBase::onMouse(x, y, state, modifiers);
    }
    return handled;
}

bool Chart::onKey(sia::KeyBoard key, sia::InputState state, uint32_t modifiers) {
    if (d->panels.empty()
            || !d->flags.test(Chart::kFunctionKeyInput)) {
        return false;
    }

    switch (key) {
    case KeyBoard::kLeft:
    case KeyBoard::kRight:
        if (modifiers & ModifierKey::kControl_ModifierKey) {
            // 移动视图
            if (d->flags.test(Chart::kFunctionKeyMoveView)) {
                moveView((key == KeyBoard::kLeft) ? -1 : 1);
            }
        } else {
            if (d->flags.test(Chart::kFunctionCrossHair) && d->cross.flag(CrossHair::kHasCross)) {
                return d->cross.onKey(key, state, modifiers);
            }
        }
        break;

    case KeyBoard::kUp:
        if (d->flags.test(Chart::kFunctionKeyZoom)) {
            zoomIn();
        }
        break;

    case KeyBoard::kDown:
        if (d->flags.test(Chart::kFunctionKeyZoom)) {
            zoomOut();
        }
        break;

    case KeyBoard::kDelete:
        if (d->selected) {
            // 删除item
            if (Panel *p = d->selected->panel()) {
                p->removeViewitem(d->selected);
            }
        }
        break;
    }
    return true;
}

void Chart::onSizeChange() {
    ChartBase::onSizeChange();

    // 将cross覆盖到其上
    d->cross.setGeometry(rect());
    d->flags.set(ChartPrivate::kViewDirty, true);
}

void Chart::onSurfaceChanged() {
    d->vcache.surf = surface();
    d->flags.set(ChartPrivate::kViewDirty, true);
}

}
}

