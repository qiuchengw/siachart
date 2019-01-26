#pragma once

#include "chartbase.h"
#include "bitflag.h"

namespace sia {
namespace stockchart {
/*!
 * \class Axis
 *
 * \brief 图表坐标轴基类
 *
 *
 */
class MarketChart : public StockChartBase {
    enum Flags {
    };

public:
    //! 序列名
    static const SkString PRICE_LINE;
    static const SkString AVG_PRICE_LINE;
    static const SkString VOL_BAR;

    MarketChart();

    /*! \brief 设置开盘价
     *
     *    @param	price	开票价格
     *    @return
     */
    void setOpenPrice(chartbase::data_val_t price);

    /*! \brief 价格显示档位
     *
     *    @param	count	显示等分的价格的档位
     *    @return
     */
    void setValueSlice(int count);

    HPANEL mainPanel()const {
        return main_panel_;
    }

    HPANEL volPanel()const {
        return vol_panel_;
    }

protected:
    void initPanels();

private:
    HPANEL main_panel_ = nullptr;
    HPANEL vol_panel_ = nullptr;
    BitFlags flags_;
};
}
}
