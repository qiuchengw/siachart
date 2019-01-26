#include "mychart.h"
#include "utils.h"
#include "tdxdata.h"

static std::default_random_engine _rand_gen;
inline int32_t rand_v(int32_t vmin, int32_t vmax) {
    // 最新价格在 涨跌幅 [5, 100] / 10000 之间
    std::uniform_int_distribution<int> dis(vmin, vmax);
    return dis(_rand_gen);
}

MyChart::MyChart()
    :Chart(SkString("mychart")) {
    //    setCrossHairDetail(&sto_detail_);
    setCalendarFormat(SkString("%H:%M"), SkString("%Y/%m/%d, %H:%M"));
    // setValueSlice(10);

    main_ = addPanel();
    vol_ = addPanel();
    pline_ = addPanel();
    setValueSlice(main_, 11);

    initData();
    initPanel();
    initObject();
}

void MyChart::initData() {
//    setMiddleVal(main_, 10000);

//    std::map<chart_key_t, chart_val_t> d_open;
//    std::map<chart_key_t, chart_val_t> d_high;
//    std::map<chart_key_t, chart_val_t> d_low;
//    std::map<chart_key_t, chart_val_t> d_close;

//    chart_key_t tm;
//    utils::timed::forEveryDay(20181001, 20181003, [&](int32_t d) {
//        utils::timed::forEveryMinute(93000, 120000, [&](int32_t t) {
//            if (t % 500) {
//                return;
//            }

//            tm = chart_key_t(d) * 1000000000 + t*1000;

//            tms_.push_back(tm);

//            // 模拟A股市场，价格范围在[9, 11]
//            auto open = rand_v(9500, 10500);
//            auto close = rand_v(9500, 10500);
//            auto low = rand_v(9000, std::min(close, open));
//            auto high = rand_v(std::max(open, close), 11000);

//            d_low.insert({ tm, low });
//            d_open.insert({ tm, open });
//            d_close.insert({ tm,  close});
//            d_high.insert({ tm, high });
//        });
//    });

    TdxData tdx;
    KLine kline;
    if (tdx.loadKLine("./600000.txt", kline)) {
        setSymbol(kline.symbol);

        addData(SkString("open"), kline.open);
        addData(SkString("high"), kline.high);
        addData(SkString("low"), kline.low);
        addData(SkString("close"), kline.close);
    }

    setCrossHairDataname(SkString("open"));
}

int32_t MyChart::showFormula(const SkString &formula_name, const formula::TaParam &param) {
    if (formula_ && formula_id_ > 0) {
        removeFormula(formula_, formula_id_);
    }

    if (!formula_) {
        formula_ = addPanel();
    }

    // settings for MACD
//    formula::TaParam para;
//    para.setParam(SkString("optInFastPeriod"), 12);
//    para.setParam(SkString("optInSlowPeriod"), 26);
//    para.setParam(SkString("optInSignalPeriod"), 9);

//    // 设置输出图形
//    chartbase::TaSeries ts;
//    ts.color = SK_ColorBLUE;
//    ts.data_name.push_back(SkString("outMACD"));
//    ts.series_name = SkString("fast_p");
//    ts.typ = SeriesType::kLine;
//    para.addOutSeries(ts);

//    ts.color = SK_ColorCYAN;
//    ts.data_name.reset();
//    ts.data_name.push_back(SkString("outMACDHist"));
//    ts.series_name = SkString("slow_p");
//    ts.typ = SeriesType::kUpdownBar;
//    para.addOutSeries(ts);

//    ts.color = SK_ColorWHITE;
//    ts.data_name.reset();
//    ts.data_name.push_back(SkString("outMACDSignal"));
//    ts.series_name = SkString("signal_p");
//    ts.typ = SeriesType::kLine;
//    para.addOutSeries(ts);
//    showFormula(SkString("MACD"), para);


    formula_id_ = Chart::showFormula(formula_, formula_name, param);
    return formula_id_;
}

int32_t MyChart::addFormula(HPANEL panel, const SkString &formula_name, const formula::TaParam &param) {
    if (!panel) {
        return -1;
    }
    return Chart::showFormula(panel, formula_name, param);
}

void MyChart::addDemoSeries(HPANEL panel, SeriesType typ) {
//    static int
}

void MyChart::initPanel() {

    std::map<SkString, SkColor> clrs = {
        { SkString("line_test"), SK_ColorYELLOW},
        { SkString("bar_test"), SK_ColorGREEN},
        { SkString("candle_test"), SK_ColorCYAN},
    };

    // lines
    SkTArray<SkString> line;
    line.push_back(SkString("open"));
    line.push_back(SkString("close"));
    addSeries(pline_, SkString("line_test"), SeriesType::kLine, line);
//    setSeriesColor(pline_, clrs);
    setPrecision(pline_, 2, 1);

    // candles
    SkTArray<SkString> candle;
    candle.push_back(SkString("open"));
    candle.push_back(SkString("high"));
    candle.push_back(SkString("low"));
    candle.push_back(SkString("close"));
    addSeries(main_, SkString("candle_test"), SeriesType::kCandle, candle);
    setPrecision(main_, 2, 1);

//    setSeriesColor(main_, clrs);

    auto cb = [](SeriesTooltipData& d)->bool {
        if (d.key % 30 == 0) {
            return false;
        }

        d.title = d.series_name;
        for (auto i : d.datas) {
            SeriesTooltipData::Item item;
            SkString val = d.formatVal(i.second);
            item.cont.printf("%s: %s", i.first.c_str(), val.c_str());
            item.color = Sk_InvalidColor;

            d.items.push_back(item);
        }
        return true;
    };
    std::map<SkString, SeriesTooltipCallback> cbs;
    cbs.insert({SkString("candle_test"), cb});
    setSeriesTooltipCallback(main_, cbs);

    // bars
    SkTArray<SkString> bar;
    bar.push_back(SkString("close"));
    addSeries(vol_, SkString("bar_test"), SeriesType::kBar, bar);
//    setSeriesColor(vol_, clrs);
    setPrecision(vol_, 2, 1);

    setCurrentDomain(0, -1);
}

void MyChart::EditData() {
    static const std::vector<SkString> _datanames = {
        SkString("open"),
        SkString("high"),
        SkString("low"),
        SkString("close"),
    };

    std::map<chart_key_t, chart_val_t> vals;
    SkString name = _datanames[rand_v(0, _datanames.size() - 1)];
    for (int i = 0; i < tms_.size() / 3; ++i) {
        int idx = rand_v(0, tms_.size() - 1); // 随机改变一个时间上的值
        vals.insert({ tms_[i], rand_v(9000, 11000) });
    }
    addData(name, vals);
}

void MyChart::initObject() {
    addPicObject();
    addTextObject();
}

void MyChart::addTextObject() {
    return;

    my_sp<IVisualObject> p = IVisualObject::create(ObjectType::kText);
    if (TextObject* po = dynamic_cast<TextObject*>(p.get())) {
        po->font_face = SkString("宋体");
        // po->font_style = SkFontStyle::BoldItalic();
        po->text = SkString(u8"Hello 中国");
        // po->text_size = 18;
        po->color = SK_ColorMAGENTA;
    }

    ObjectParam op;
    op.postyp = PosType::kAttached;
    op.attach = SkString("bar_test");
    op.x = tms_[rand_v(0, tms_.size() - 1)];
    op.sz = SkSize::Make(60, 20);
    addObject(vol_, SkString("text_obj"), op, p);

    // 搞一个右上角，大的
    my_sp<IVisualObject> p1 = IVisualObject::create(ObjectType::kText);
    if (TextObject* po = dynamic_cast<TextObject*>(p1.get())) {
        po->font_face = SkString("宋体");
        // po->font_style = SkFontStyle::BoldItalic();
        po->text = SkString(u8"Hello world\n中国人民从此站起来了\nand are you ok\n鸟好，my friend");
        // po->text_size = 18;
        po->color = SK_ColorMAGENTA;
    }
    ObjectParam op1;
    op1.relpos.r = 30.f;
    op1.relpos.t = 10.f;
    op1.postyp = PosType::kRelated;
    op1.sz = SkSize::Make(100, 100);
    op1.tooltip = SkString(u8"这个是tip提示");
    addObject(main_, SkString("big_text_obj"), op1, p1);
}

void MyChart::addPicObject() {
    my_sp<IVisualObject> p = IVisualObject::create(ObjectType::kPicture);
    if (PictrueObject* po = dynamic_cast<PictrueObject*>(p.get())) {
        po->path = SkString("d:/chart.jpg");
    }

    ObjectParam op;
    op.postyp = PosType::kRelated;
    // op.sz = SkSize::Make();

    RelPos& rp = op.relpos;
    rp.l = 100.f;
    rp.t = 100.f;
    //         rp.r = 100.f;
    //         rp.b = 100.f;
    op.sz = SkSize::Make(50, 50);
    addObject(main_, SkString("image"), op, p);
}

