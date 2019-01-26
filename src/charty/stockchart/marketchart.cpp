#include "marketchart.h"

namespace sia {

namespace stockchart {

const SkString MarketChart::PRICE_LINE = SkString("price");
const SkString MarketChart::AVG_PRICE_LINE = SkString("avg_price");
const SkString MarketChart::VOL_BAR = SkString("volume");

MarketChart::MarketChart()
    :StockChartBase(SkString("stockchart-market")) {
    initPanels();

    // 禁用放大/缩小
    setFunction(Chart::kFunctionRubberSelect | Chart::kFunctionKeyZoom | Chart::kFunctionKeyMoveView, false);
}

void MarketChart::setOpenPrice(chartbase::data_val_t price) {
    setMiddleVal(main_panel_, price);
}

void MarketChart::setValueSlice(int count) {
    StockChartBase::setValueSlice(main_panel_, count);
}

void MarketChart::initPanels() {
    main_panel_ = addPanel();
    std::map<SkString, charty::SeriesStyle> styles = {
        { PRICE_LINE,  charty::SeriesStyle(SkIntToScalar(2), SK_ColorWHITE)},
        { AVG_PRICE_LINE, charty::SeriesStyle(SK_ColorYELLOW) },
        { VOL_BAR, charty::SeriesStyle(true, SK_ColorCYAN, SkIntToScalar(0)) },
    };

    // 主panel两条线：均价和实时价格
    addSeries(main_panel_, PRICE_LINE, charty::SeriesType::kLine, PRICE_LINE);
    addSeries(main_panel_, AVG_PRICE_LINE, charty::SeriesType::kLine, AVG_PRICE_LINE);
    setSeriesStyle(main_panel_, styles);
    setPrecision(main_panel_, 2, 10000);	// 默认价格乘以10000
    showGrid(main_panel_, true, true);
    setValueSlice(14);

    setGridVerticalSlice(main_panel_, 8);
    showSeriesMinMaxTip(main_panel_, PRICE_LINE, true);

    // 成交量面板
    vol_panel_ = addPanel();
    addSeries(vol_panel_, VOL_BAR, charty::SeriesType::kBar, VOL_BAR);
    setSeriesStyle(vol_panel_, styles);
    setPrecision(vol_panel_, 0, 100);	// 默认数量乘以100
    showGrid(vol_panel_, true, false);
    showPanelTitle(vol_panel_, false);

    // 默认显示全部的数据
    setCurrentDomain(0, -1);

    // 十字线导航的时候贴合到price上
    setCrossHairDataname(MarketChart::PRICE_LINE);

}

}



}

