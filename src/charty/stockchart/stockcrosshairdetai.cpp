#include "stockcrosshairdetai.h"

namespace sia {

namespace stockchart {

std::vector<MarketCrossDetail::DetailItem> MarketCrossDetail::detailItems(
    chartbase::data_key_t k, const std::map<SkString, chartbase::data_val_t>& data) {

    using namespace sia::charty;
    std::vector<DetailItem> ret;
    DetailItem hd;
    ret.push_back(DetailItem(SkString(u8"Ê±¼ä"), formatKey(k), SK_ColorCYAN));

    for (auto i : data) {
        hd.title = i.first;
        hd.val = formatVal(i.second);
        ret.push_back(hd);
    }
    return ret;
}

}



}

