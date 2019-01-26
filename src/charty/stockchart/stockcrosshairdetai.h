#pragma once

#include "chartbase.h"
#include "../charty/crosshairdetail.h"

namespace sia {

// namespace charty {
//     class CrossHairDetail;
//     struct CrossHairDetailItem;
// }

namespace stockchart {

class MarketCrossDetail : public charty::CrosshairDetail {
public:
    virtual std::vector<DetailItem> detailItems(chartbase::data_key_t k, const std::map<SkString, chartbase::data_val_t>& data) override;
};

}
}
