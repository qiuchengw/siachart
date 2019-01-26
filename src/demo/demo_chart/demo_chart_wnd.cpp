#if defined vsnprintf
#undef vsnprintf
#endif

#include <vector>
#include <iostream>
#include <random>
#include "../mgraphy/scenewnd.h"
#include "charty/stockchart/stockcrosshairdetai.h"
#include "charty/formula/talib.h"
#include "charty/charty/viewobject.h"
#include "utils.h"
#include "demo_quote_chart_wnd.h"
#include "charty/charty/chartman.h"

using namespace sia::formula;
using namespace sia::charty;
using namespace sia::stockchart;

class DemoChartWnd : public SceneWnd {
public:
    DemoChartWnd(HINSTANCE hinst) {
        quote_wnd_.init(hinst);
        quote_wnd_.show();
    }

    virtual sia::graphy::Scene* createScene() override {
        installDictionary();

        chart_ = new sia::stockchart::StockChartBase(SkString("demo_chart"));
        chart_->setCrossHairDetail(&sto_detail_);
        chart_->setCalendarFormat(SkString("%H:%M"), SkString("%Y/%m/%d, %H:%M"));

        main_ = chart_->addPanel();
        vol_ = chart_->addPanel();
        pline_ = chart_->addPanel();
        chart_->setSymbol(SkString(u8"天天快递"));
        chart_->setValueSlice(main_, 13);

        // chart_->setFunction(Chart::kFunctionTooltip, false);

        initData();
        initPanel();
        initObject();

        // 指标
        formulaToggle(true);
        return chart_;
    }

    void initData() {
        chart_->setMiddleVal(main_, 10000);

        std::map<chart_key_t, chart_val_t> d_open;
        std::map<chart_key_t, chart_val_t> d_high;
        std::map<chart_key_t, chart_val_t> d_low;
        std::map<chart_key_t, chart_val_t> d_close;

        chart_key_t tm;
        utils::timed::forEveryDay(20181201, 20181204, [&](int32_t d) {
            utils::timed::forEveryMinute(93000, 120000, [&](int32_t t) {
                tm = chart_key_t(d) * 1000000000 + t*1000;

                tms_.push_back(tm);
                d_low.insert({ tm, rand_v(9000, 10000) });
                d_open.insert({ tm, rand_v(9500, 10500) });
                d_close.insert({ tm, rand_v(9500, 10500) });
                d_high.insert({ tm, rand_v(10000, 11000) });
            });
        });

        std::cout << tms_.size() << std::endl;

        chart_->addData(SkString("open"), d_open);
        chart_->addData(SkString("high"), d_high);
        chart_->addData(SkString("low"), d_low);
        chart_->addData(SkString("close"), d_close);
        chart_->setCrossHairDataname(SkString("open"));
    }

    void initPanel() {

        std::map<SkString, SkColor> clrs = {
            { SkString("line_test"), SK_ColorYELLOW},
            { SkString("bar_test"), SK_ColorGREEN},
            { SkString("candle_test"), SK_ColorCYAN},
        };

        // lines
        SkTArray<SkString> line;
        line.push_back(SkString("close"));
        line.push_back(SkString("open"));
        chart_->addSeries(pline_, SkString("line_test"), SeriesType::kLine, line);
        // chart_->setSeriesColor(pline_, clrs);
        chart_->setPrecision(pline_, 2, 1000);
        SkTArray<PanelTitleItem> line_title;
        PanelTitleItem ti;
        ti.item = SkString("line_test");
        ti.typ = PanelTitleItemType::kSeriesName;
        line_title.push_back(ti);
        chart_->setTitleItems(pline_, line_title);

        // candles
        SkString canle_name("candle_test");
        SkTArray<SkString> candle;
        candle.push_back(SkString("open"));
        candle.push_back(SkString("high"));
        candle.push_back(SkString("low"));
        candle.push_back(SkString("close"));
        chart_->addSeries(main_, canle_name, SeriesType::kCandle, candle);
        chart_->setPrecision(main_, 2, 1000);
        // chart_->setSeriesColor(main_, clrs);
        chart_->showSeriesMinMaxTip(main_, canle_name, true);

        SkTArray<PanelTitleItem> candle_title;
        ti.typ = PanelTitleItemType::kChartSymbol;
        candle_title.push_back(ti);
        ti.item = SkString("candle_test");
        ti.typ = PanelTitleItemType::kSeriesName;
        candle_title.push_back(ti);
        ti.item = SkString(u8"卧槽！Fuck!");
        ti.typ = PanelTitleItemType::kConstant;
        candle_title.push_back(ti);

        // chart_->setTitleItems(main_, candle_title);


        auto cb = [](SeriesTooltipData& d)->bool {
//             if (d.key % 30 == 0) {
//                 return false;
//             }

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
        chart_->setSeriesTooltipCallback(main_, cbs);

        // bars
        SkTArray<SkString> bar;
        bar.push_back(SkString("close"));
        chart_->addSeries(vol_, SkString("bar_test"), SeriesType::kBar, bar);
        // chart_->setSeriesColor(vol_, clrs);
        chart_->setPrecision(vol_, 0, 100);

        chart_->setCurrentDomain(0, -1);
    }

    void EditData() {
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
        chart_->addData(name, vals);

    }

    void formulaToggle(bool on) {
        // Talib
        formula::TaParam para;
        para.setParam(SkString("optInFastPeriod"), 12);
        para.setParam(SkString("optInSlowPeriod"), 26);
        para.setParam(SkString("optInSignalPeriod"), 9);

        // 设置输出图形
        TaSeries ts;
//         ts.color = SK_ColorBLUE;
//         ts.data_name.push_back(SkString("outMACD"));
//         ts.series_name = SkString("fast_p");
//         ts.typ = SeriesType::kLine;
//         para.addOutSeries(ts);
//
//         ts.color = SK_ColorCYAN;
//         ts.data_name.reset();
//         ts.data_name.push_back(SkString("outMACDHist"));
//         ts.series_name = SkString("slow_p");
//         ts.typ = SeriesType::kUpdownBar;
//         para.addOutSeries(ts);
//
//         ts.color = SK_ColorWHITE;
//         ts.data_name.reset();
//         ts.data_name.push_back(SkString("outMACDSignal"));
//         ts.series_name = SkString("signal_p");
//         ts.typ = SeriesType::kLine;
//         para.addOutSeries(ts);

        if (!formula_panel_) {
            formula_panel_ = chart_->addPanel();
        }

        static int32_t _id = 0;
        if (on) {
            _id = chart_->showFormula(formula_panel_, SkString("MACD"), para);
        } else {
            chart_->removeFormula(formula_panel_, _id);
        }
    }

    void initObject() {
        addPicObject();
        addTextObject();
    }

    void addTextObject() {
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
        chart_->addObject(vol_, SkString("text_obj"), op, p);

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
        chart_->addObject(main_, SkString("big_text_obj"), op1, p1);
    }

    void addPicObject() {
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
        chart_->addObject(main_, SkString("image"), op, p);
    }

    void installDictionary() {
        auto dic = ChartMan::instance()->dict();
        dic->add(SkString("line_test"), SkString(u8"价格线"));
        dic->add(SkString("price"), SkString(u8"分时走势"));
        dic->add(SkString("avg_price"), SkString(u8"均线"));
        dic->add(SkString("line_test"), SkString(u8"价格线"));
        dic->add(SkString("bar_test"), SkString(u8"买卖量"));
        dic->add(SkString("candle_test"), SkString(u8"蜡烛图"));
        dic->add(SkString("open"), SkString(u8"开"));
        dic->add(SkString("high"), SkString(u8"高"));
        dic->add(SkString("low"), SkString(u8"低"));
        dic->add(SkString("close"), SkString(u8"平"));
    }

protected:
    virtual bool onMouse(uint16_t x, uint16_t y, sia::InputState state, uint32_t mod) override {
        if (state == InputState::kDown_InputState ) {
            std::cout << "mod:" << mod << std::endl;
            if (mod & sia::kControl_ModifierKey) {
                static bool _fml = true;
                // formulaToggle(_fml);

                // 把最后一个panel合并到第一个上去
                int n = chart_->panelCount();
                if (n >= 2) {
                    chart_->mergePanel(chart_->panel(0), chart_->panel(n - 1));
                }

                // chart_->showPanelItem(/*PanelItemType::kPanelValueAxis |*/ PanelItemType::kPanelTitle, _fml);
                _fml = !_fml;

//                 SeriesStyle ss = siaSeriesStyle();
//                 ss.fill_bar = !ss.fill_bar;
//                 ss.antialias = !ss.antialias;
//                 setSiaSeriesStyle(ss);
//
//                 SkString theme = ss.fill_bar ? SkString("black_theme.lua") : SkString("white_theme.lua");
//                 SkString err;
//                 ChartMan::instance()->loadThemeConfig(theme, err);

                // chart_->removePanel(chart_->panel(1));
            }
        }
        return SceneWnd::onMouse(x, y, state, mod);
    }

private:
    sia::stockchart::StockChartBase* chart_ = nullptr;
    sia::stockchart::MarketCrossDetail sto_detail_;
    HPANEL main_ = nullptr;
    HPANEL vol_ = nullptr;
    HPANEL pline_ = nullptr;

    HPANEL formula_panel_ = nullptr;

    std::vector<chart_key_t> tms_;

    QuoteWnd quote_wnd_;
};

void setSeriesStyle() {
    SeriesStyle ss;
    //         ss.line_cap = SkPaint::kRound_Cap;
    //         ss.line_join = SkPaint::kRound_Join;
    ss.stroke_width = 1;
    ss.down_color = SK_ColorGREEN;
    ss.up_color = SK_ColorRED;
    ss.fill_bar = false;
    setSiaSeriesStyle(ss);
}

CWindow* create_wnd(HINSTANCE inst) {
    setSeriesStyle();

// 	auto *p = new DemoChartWnd(inst);
// 	p->init(inst);
// 	p->show();
//
// 	p = new DemoChartWnd(inst);
// 	p->init(inst);
// 	p->show();

    return new DemoChartWnd(inst);
    // return new QuoteWnd(inst);
}

