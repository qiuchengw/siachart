#include "panel.h"
#include <cassert>

#include "defines.h"
#include "title.h"
#include "valueaxis.h"
#include "ratioaxis.h"
#include "mainview.h"
#include "calendaraxis.h"
#include "series.h"
#include "../formula/talib.h"
#include "../formula/tadata.h"
#include "../formula/iformula.h"
#include "chartman.h"
#include "seriesname.h"
#include "viewitem.h"
#include "private.h"

namespace sia {

namespace charty {

Panel::Panel(graphy::Widget* parent)
    :VisualItem(SkString("panel"), parent) {
    setZIndex(VisualItemZValue::kPanel);

    flags_.set(PanelItemType::kPanelAll);

    initItems();
}

Panel::~Panel() {
}

void Panel::recalc(Bits flags) {
    for (auto w : children()) {
        if (auto *v = dynamic_cast<VisualItem*>(w)) {
            v->recalc(flags);
        }
    }

    if (flags & ChartPrivate::kDataDirty) {
        refreshFormulas();
    }
}

void Panel::views(SkTArray<my_sp<MainView>>& out) const {
    out.push_back(mainview_);
    for (auto p : subviews_) {
        out.push_back(p);
    }
}

std::vector<my_sp<Series>> Panel::serieses() const {
    std::vector<my_sp<Series>> ret;
    auto viewSeries = [](std::vector<my_sp<Series>>& v, my_sp<MainView> view) {
        for (auto item : view->serieses()) {
            v.push_back(item);
        }
    };
    viewSeries(ret, mainview_);

    for (auto v : subviews_) {
        viewSeries(ret, v);
    }
    return ret;
}

my_sp<Series> Panel::series(const SkString& series_name, my_sp<MainView>& view) const {
    my_sp<Series> ret = nullptr;
    auto viewSeries = [](my_sp<MainView> view, const SkString& sname)->my_sp<Series> {
        for (auto item : view->serieses()) {
            if (sname.equals(item->seriesName())) {
                return item;
            }
        }
        return nullptr;
    };
    if (ret = viewSeries(mainview_, series_name)) {
        view = mainview_;
        return ret;
    }

    for (auto v : subviews_) {
        if (ret = viewSeries(v, series_name)) {
            view = v;
            return ret;
        }
    }
    return nullptr;
}

bool Panel::createFormulaSeries(const SkString& formula_name, const int32_t id, Panel::_Formula& fml,
                                const SkTArray<formula::TaSeries>* p_series, const formula::TaParam& user_param) {

    // 指标brief名
    fml.brief_name = SkStringPrintf("%s%s", formula_name.c_str(), user_param.paramBrief(false).c_str());

    // 创建一个序列的代理函数
    std::map<SkString, SkColor> series_colors;
    auto createMySeries = [&](const SkString& series_name, const formula::TaSeries& ts) {
        fml.series_names.push_back(series_name);
        SkTArray<SkString> data_names;
        for (auto& ss : ts.data_name) {
            data_names.push_back(SeriesName::formulaDataname(formula_name, id, ss));
        }

        if (auto ss = fml.view->addSeries(series_name, ts.typ, data_names)) {
            // 要求此序列删除的时候，同时删除掉其关联的数据
            ss->setRemoveDataOnDelete(true);

            // 在title上显示
            PanelTitleItem title;
            title.item = series_name;
            title.typ = PanelTitleItemType::kSeriesName;
            title_.addTitleItem(title);
        }
        series_colors[series_name] = ts.color;
    };

    // 当仅有一个输出序列的时候，就不用添加PanelTitleItemType::kConstant类型的标题了。
    // 我们把指标名 item($para)当作其序列名
    bool only_1_out = (p_series->count() == 1);
    bool use_user_config = !(user_param.outSeries().empty());
    if (only_1_out) {
        if (!use_user_config) { // 用户未提供配置
            SkString series_name = fml.brief_name;
            createMySeries(series_name, p_series->back());
            return true;
        }
    } else {
        // 先在最前面加上一个指标的brief title
        if (!use_user_config) {
            // 在title上显示
            PanelTitleItem title;
            title.item = fml.brief_name;
            title.typ = PanelTitleItemType::kConstant;
            title_.addTitleItem(title);
        }
    }

    // 创建多个序列
    for (const formula::TaSeries& ts : *p_series) {
        SkString series_name = SeriesName::formulaSeriesName(formula_name, ts.series_name, id);
        createMySeries(series_name, ts);
    }
    fml.view->setSeriesColor(series_colors);

    return true;
}

int32_t Panel::findFormulaBySeriesName(const SkString& series_name) const {
    for (auto i : formulas_) {
        if (i.second.hasSeries(series_name)) {
            return i.first;
        }
    }
    return -1;
}

int32_t Panel::showFormula(const SkString& formula_name, const formula::TaParam& param) {
    using namespace sia::formula;

    //! 凡是添加到图表中的每个公式都必须有一个唯一的id
    static int32_t _id = 1;
    if (auto ctx = chartCtx()) {
        // 先创建公式实例
        auto indi = ChartMan::instance()->createFormula(formula_name);
        // 向公式里面灌输数据
        // TODO：fix 性能问题？对于公式，我们总是计算所有的数据？
        if (indi
                && indi->useDataSource(datasource(), 0, ctx->totalIndexCount() - 1)	// 灌入数据源的数据
                && indi->calc(param)) {	// 计算

            // 我们先看下，是否有提供输出序列配置。
            auto& user_series = param.outSeries();
            auto *p_series = &user_series;
            bool use_user_config = true;
            SkTArray<formula::TaSeries> suggest_series;
            if (user_series.empty()) {
                // 如果用户没有明确设定序列的话，那么就按照公式建议的序列来设定好了
                if (indi->suggestOutSeries(suggest_series)) {
                    p_series = &suggest_series;
                    use_user_config = true;
                }
            }

            // 如果没有输出配置，那么这个公式也没用
            if (!p_series || p_series->empty()) {
                ChartMan::instance()->destroyFormula(indi);
                return -1;
            }

            // 每个公式一个唯一id
            _id++;

            // 记录公式的输出数据名
            _Formula fml;
            fml.view = mainview_;
            fml.fml = indi;
            fml.id = _id;
            fml.formula_name = formula_name;
            fml.param = param;

            // 把数据添加到数据源里面！
            const formula::TaData& dat = indi->data();
            dat.forValidOutData([&](const SkString& data_name, const std::map<chartbase::data_key_t, chartbase::data_val_t>& d)->bool {
                SkString dname = SeriesName::formulaDataname(formula_name, _id, data_name);
                fml.data_names.push_back(dname);
                ctx->addData(dname, d);
                return true;
            });

            // 创建输出序列
            if (createFormulaSeries(formula_name, _id, fml,p_series, param)) {
                // 记录下来所有的指标
                formulas_.insert({ _id, fml });

                return _id;
            }
        }

        if (indi) {
            // 销毁
            ChartMan::instance()->destroyFormula(indi);
        }
    }
    return -1;
}

void Panel::removeFormula(int32_t id) {
    auto i = formulas_.find(id);
    if (i == formulas_.end()) {
        return;
    }

    // 删除序列
    const _Formula& fml = i->second;
    // 从Title中移除Item
    title_.removeTitleItem(fml.brief_name);

    // 删除序列。
    // 在创建的时候已设置：Series::setRemoveDataOnDelete(true)
    for (const SkString& s : fml.series_names) {
        fml.view->removeSeries(s);
    }
    formulas_.erase(i);
}

void Panel::showItem(int item, bool show) {
    if (item & PanelItemType::kPanelCalendar) {
        showPanelItem<CalendarAxis>(calendar_, PanelItemType::kPanelCalendar, show);
    }

    if (item & PanelItemType::kPanelTitle) {
        title_.setBlank(!show);
        flags_.set(PanelItemType::kPanelTitle, show);
        // showPanelItem<Title>(title_, PanelItemType::kPanelTitle, show);
    }

    if (item & PanelItemType::kPanelValueAxis) {
        showPanelItem<ValueAxis>(value_, PanelItemType::kPanelValueAxis, show);
    }

    if (item & PanelItemType::kPanelRatioAxis) {
        showPanelItem<RatioAxis>(ratio_, PanelItemType::kPanelRatioAxis, show);
    }

    // 重新计算布局
    relayout();
}

void Panel::seriesRemoved(const SkString& series_name) {
    title_.removeTitleItem(series_name);

    // 检查副图，如果副图的内容为空的画，需要把副图删掉
    auto i = std::remove_if(subviews_.begin(), subviews_.end(), [](my_sp<MainView> view)->bool {
        return view->viewEmpty();
    });
    // 移除这个视图
    subviews_.pop_back_n(int(subviews_.end() - i));
}

bool Panel::mergePanel(Panel* sub) {
    if (!sub) {
        return false;
    }

    // 合并title显示
    if (Title* sub_title = sub->title()) {
        for (auto& itm : sub_title->titleItems()) {
            title_.addTitleItem(itm);
        }
    }

    // 合并指标
    for (auto i : sub->formulas()) {
        formulas_.insert(i);
    }

    SkTArray<my_sp<MainView>> sub_views;
    sub->views(sub_views);
    // 合并子视图
    for (auto p : sub_views) {
        p->setAsSubview(true);
        p->setParent(this);
        subviews_.push_back(p);
    }

    return true;
}

void Panel::removeViewitem(ViewItem* item) {
    if (MainView* view = item->mainView()) {
        switch (item->viewType()) {
        case ViewItemType::kViewObject: {
            view->removeObject(item->name());

            break;
        }

        case ViewItemType::kViewSeries: {
            if (auto p = dynamic_cast<Series*>(item)) {
                SkString series_name = p->seriesName();
                view->removeSeries(series_name);

                // 如果是指标输出的序列，那么要删除这个公式
                int32_t id = findFormulaBySeriesName(series_name);
                if (-1 != id) {
                    removeFormula(id);
                }
            }
        }
        break;
        }
    }
}

void Panel::initItems() {
    showItem(PanelItemType::kPanelCalendar, flags_.test(PanelItemType::kPanelCalendar));
    // showItem(PanelItemType::kPanelTitle, flags_.test(PanelItemType::kPanelTitle));
    showItem(PanelItemType::kPanelRatioAxis, flags_.test(PanelItemType::kPanelRatioAxis));
    showItem(PanelItemType::kPanelValueAxis, flags_.test(PanelItemType::kPanelValueAxis));

    // title 不允许删除
    initPanelItem(&title_);

    // 主视图
    if (!mainview_) {
        mainview_ = std::make_shared<MainView>();
        initPanelItem(mainview_.get());
    }

    layout_.setPanel(this);
    setLayout(&layout_);
}

void Panel::initPanelItem(VisualItem* w) {
    this->addChild(w);
    w->setChartContext(chartCtx());
}

bool Panel::onMouse(int x, int y, sia::InputState state, uint32_t modifiers) {
    if (subviews_.empty()) {
        return false;
    }

    // 原始坐标
    SkPoint pt = mainview_->windowToClient(SkPoint::Make(SkIntToScalar(x), SkIntToScalar(y)));
    // 如果是多个视图叠加起来的，因为大家相互覆盖，鼠标消息只能在最上面的哪个视图中处理
    // 所以我们要把鼠标消息传递下去
    if (mainview_->onMouse(pt.x(), pt.y(), state, modifiers)) {  // 先主视图处理
        return true;
    }

    // 然后子视图
    for (auto p : subviews_) {
        if (p->onMouse(pt.x(), pt.y(), state, modifiers)) {
            return true;
        }
    }
    return false;
}

void Panel::refreshFormulas() {
    return;

    std::unordered_map<int32_t, _Formula> tmp = formulas_;
    for (auto i : tmp) {
        const _Formula& fml = i.second;

        // 删除指标
        removeFormula(i.first);

        // 重建指标
        showFormula(fml.formula_name, fml.param);
    }
}

}
}

