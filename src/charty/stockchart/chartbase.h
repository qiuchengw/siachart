#pragma once

#include "../charty/chart.h"

namespace sia {
namespace stockchart {
/*!
 * \class StockChartBase
 *
 * \brief 股票图表的基础类
 *
 *
 */
class StockChartBase : public charty::Chart {
public:
    StockChartBase(const SkString& id);

private:

};
}
}
