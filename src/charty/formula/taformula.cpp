#include "taformula.h"

namespace sia {
namespace formula {

TaFormula::TaFormula(const SkString& name)
    :IFormula(name) {
    ta_item_.setName(name);
    ta_item_.init();
}

bool TaFormula::calc(const TaParam& para) {
    if (ta_item_.valid()) {
        ta_item_.setData(&data_);

        // ÷¥––º∆À„
        return ta_item_.calc(para);
    }
    return false;
}

bool TaFormula::queryParam(SkTArray<TaParamItem>& para) {
    if (ta_item_.valid()) {
        return ta_item_.queryParam(para);
    }
    return false;
}

bool TaFormula::defaultParam(TaParam& para) {
    if (ta_item_.valid()) {
        return ta_item_.defaultParam(para);
    }
    return false;
}

bool TaFormula::suggestOutSeries(SkTArray<TaSeries>& series) {
    if (ta_item_.valid()) {
        return ta_item_.suggestOutSeries(series);
    }
    return false;
}

}
}

