#if defined vsnprintf
#undef vsnprintf
#endif

#include <vector>
#include <iostream>
#include <random>
#include "charty/stockchart/marketchart.h"
#include "../mgraphy/scenewnd.h"
#include "charty/stockchart/stockcrosshairdetai.h"
#include "charty/formula/talib.h"
#include "charty/charty/viewobject.h"
#include "utils.h"

using namespace sia::formula;
using namespace sia::charty;
using namespace sia::stockchart;

static std::default_random_engine _rand_gen;
inline int32_t rand_v(int32_t vmin, int32_t vmax) {
    // 最新价格在 涨跌幅 [5, 100] / 10000 之间
    std::uniform_int_distribution<int> dis(vmin, vmax);
    return dis(_rand_gen);
}

class QuoteWnd : public SceneWnd {
public:
    QuoteWnd(HINSTANCE hinst = NULL) {
    }

    virtual sia::graphy::Scene* createScene() override {
        chart_ = new sia::stockchart::MarketChart();
        chart_->setCrossHairDetail(&sto_detail_);
        chart_->setCalendarFormat(SkString("%H:%M"), SkString("%Y/%m/%d, %H:%M"));
        chart_->setSymbol(SkString(u8"天天快递"));
        // chart_->setValueSlice(10);

        initData();
        return chart_;
    }

    void initData() {
        chart_->setOpenPrice(93500);

        std::map<chart_key_t, chart_val_t> d_price;
        std::map<chart_key_t, chart_val_t> d_avg_price;
        std::map<chart_key_t, chart_val_t> d_vol;

        chart_key_t tm;
        utils::timed::forEveryDay(20181010, 20181010, [&](int32_t d) {
            utils::timed::forEveryMinute(93000, 103000, [&](int32_t t) {
                tm = chart_key_t(d) * 1000000000 + t*1000;
                all_keys_.push_back(tm);

                // tms_.push_back(tm);
                d_vol.insert({ tm, rand_v(500000, 1000000) });
                d_price.insert({ tm, rand_v(90000, 110000) });
                d_avg_price.insert({ tm, rand_v(80000, 110000) });
            });
        });

        chart_->addData(MarketChart::PRICE_LINE, d_price);
        chart_->addData(MarketChart::AVG_PRICE_LINE, d_avg_price);
        chart_->addData(MarketChart::VOL_BAR, d_vol);

        // keys
        int keys_cnt = all_keys_.count();
// 		SkTArray<chart_key_t> keys;
// 		keys.push_back(all_keys[keys_cnt / 5]);
// 		keys.push_back(all_keys[keys_cnt / 4]);
// 		keys.push_back(all_keys[keys_cnt / 3]);
// 		keys.push_back(all_keys[keys_cnt / 2]);
// 		keys.push_back(all_keys[keys_cnt * 2 / 5]);
// 		keys.push_back(all_keys[keys_cnt * 3 / 5]);
// 		keys.push_back(all_keys[keys_cnt * 4 / 5]);
// 		chart_->setGridVerticalSlice(chart_->mainPanel(), keys, keys[keys.count() / 2]);

        // indexes
// 		SkTArray<int> indexes;
// 		indexes.push_back(keys_cnt / 5);
// 		indexes.push_back(keys_cnt / 4);
// 		indexes.push_back(keys_cnt / 3);
// 		indexes.push_back(keys_cnt / 2);
// 		indexes.push_back(keys_cnt*2 / 5);
// 		indexes.push_back(keys_cnt * 3 / 5);
// 		indexes.push_back(keys_cnt*4 / 5);
// 		chart_->setGridVerticalSlice(chart_->mainPanel(), indexes, indexes[indexes.count() / 2]);
    }

    // 删除后面5个元素
    void random_remove() {
        SkTArray<chart_key_t> keys;
        for (int i = 0; i < 5; ++i) {
            keys.push_back(all_keys_[rand_v(0, all_keys_.count() - 1)]);
        }
        chart_->removeData(MarketChart::AVG_PRICE_LINE, keys);
    }

protected:
    virtual bool onMouse(uint16_t x, uint16_t y, sia::InputState state, uint32_t mod) override {
        if (state == InputState::kDown_InputState ) {
            std::cout << "mod:" << mod << std::endl;
            if (mod & sia::kControl_ModifierKey) {
                random_remove();
            } else if (mod & sia::kShift_ModifierKey) {
                chart_->removeData(MarketChart::PRICE_LINE);
            }
        }
        return SceneWnd::onMouse(x, y, state, mod);
    }

private:
    SkTArray<chart_key_t> all_keys_;

    sia::stockchart::MarketChart* chart_ = nullptr;
    sia::stockchart::MarketCrossDetail sto_detail_;
};

