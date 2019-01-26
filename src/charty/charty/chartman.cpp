#include "chartman.h"
#include "chart.h"
#include "graphy/paintman.h"
#include "../formula/formulaman.h"

namespace sia {
namespace charty {
struct ChartManPrivate {
    ChartManPrivate() {
    }

    //! 图表实例
    std::map<SkString, Chart*> charts;

    //! 词典
    Dict dict;

    //! 公式管理
    formula::FormulaMan indiman;
};

ChartMan::ChartMan() {
    d = new ChartManPrivate();
}

ChartMan::~ChartMan() {
    delete d;
    d = nullptr;
}

Chart* ChartMan::chart(const SkString& id) const {
    for (auto i : d->charts) {
        if (id.equals(i.first)) {
            return i.second;
        }
    }
    return nullptr;
}

void ChartMan::broadcast(const SkEvent& evt) const {
    for (auto i : d->charts) {
        i.second->broadcast(evt);
    }
}

void ChartMan::regInst(Chart* c) {
    d->charts[c->ID()] = c;
}

void ChartMan::unregInst(Chart* c) {
    d->charts.erase(c->ID());
}

bool ChartMan::loadThemeConfig(const SkString& config_file, SkString& error_msg) {
    // my_sp<sia::graphy::PaintMan>
    auto man = sia::graphy::PaintMan::instance();
    if (!man) {
        man = std::make_shared<sia::graphy::PaintMan>();
        man->init();
    }

    if (!man->load(config_file)) {
        error_msg = man->errorMsg();
        return false;
    }
    man->dumpInfo();

    // 全局通知
    SkEvent evt(ChartEventType::kSiaThemeChanged);
    broadcast(evt);

    return true;
}

Dict* ChartMan::dict() {
    return &(d->dict);
}

formula::IFormula* ChartMan::createFormula(const SkString& name) {
    return d->indiman.create(name);
}

formula::FormulaMan* ChartMan::formulaMan() {
    return &(d->indiman);
}

void ChartMan::destroyFormula(formula::IFormula* inst) {
    d->indiman.destroy(inst->name(), inst);
}

//////////////////////////////////////////////////////////////////////////

static SeriesStyle __s_style_;
void setSiaSeriesStyle(const SeriesStyle& s) {
    //! 序列的通用定制
    __s_style_ = s;

    // 广播事件
    SkEvent evt(ChartEventType::kSiaSeriesStyleChanged);
    ChartMan::instance()->broadcast(evt);
}

const SeriesStyle& siaSeriesStyle() {
    return __s_style_;
}
}
}

