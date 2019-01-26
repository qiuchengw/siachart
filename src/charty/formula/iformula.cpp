#include "iformula.h"
#include "taparam.h"
#include "tadata.h"

namespace sia {
namespace formula {
struct _IndexName {
    _IndexName(TaInputDataIndex i, const SkString& n)
        :idx(i), name(n) {
    }

    TaInputDataIndex idx;
    SkString name;
};
static const std::vector<_IndexName> _indi_datanames = {
    { TaInputDataIndex::kOpen, SkString("open")},
    { TaInputDataIndex::kHigh, SkString("high")},
    { TaInputDataIndex::kLow, SkString("low")},
    { TaInputDataIndex::kClose, SkString("close")},
    { TaInputDataIndex::kVolume, SkString("volume")},
    { TaInputDataIndex::kOi, SkString("oi")},
};

IFormula::IFormula(const SkString& name)
    :name_(name) {

}

IFormula::~IFormula() {

}

bool IFormula::useDataSource(const chartbase::DataSource *ds, int idx_begin, int idx_end) {
    // 向里面灌输数据
    SkTArray<chartbase::data_key_t> keys;
    if (!ds || !ds->keys(idx_begin, idx_end, keys)) {
        return false;
    }

    data_.beginInsert(idx_end - idx_begin + 1, keys);
    ds->forIndexData(idx_begin, idx_end, [&](int32_t idx, const my_sp<chartbase::ColumnData> cd)->bool {
        for (const _IndexName& in : _indi_datanames) {
            data_.setItem(in.idx, idx, cd->get(in.name));
        }
        return true;
    });
    data_.endInsert();
    return true;
}

bool IFormula::queryParam(SkTArray<TaParamItem>& para) {
    return true;
}

bool IFormula::defaultParam(TaParam& para) {
    return true;
}

bool IFormula::suggestOutSeries(SkTArray<TaSeries>& series) {
    return false;
}

}
}

