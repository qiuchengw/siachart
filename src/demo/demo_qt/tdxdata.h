#ifndef TDXDATA_H
#define TDXDATA_H

#include <QString>

#include "../../charty/base/base.h"
#include "../../charty/charty/defines.h"

using namespace sia;
using namespace charty;
struct KLine {
    SkString symbol;
    chartbase::data_key_t  k;

    std::map<chart_key_t, chart_val_t> open;
    std::map<chart_key_t, chart_val_t> high;
    std::map<chart_key_t, chart_val_t> low;
    std::map<chart_key_t, chart_val_t> close;

//    SkTArray<chartbase::data_val_t> open;
//    SkTArray<chartbase::data_val_t> high;
//    SkTArray<chartbase::data_val_t> low;
//    SkTArray<chartbase::data_val_t> close;
};

class TdxData {
public:
    TdxData();

    bool loadKLine(const QString& file, KLine& out);
};

#endif // TDXDATA_H
