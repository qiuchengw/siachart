#ifndef MYCHART_H
#define MYCHART_H

#include "../../charty/charty/chart.h"

using namespace sia;
using namespace sia::charty;
class MyChart : public sia::charty::Chart {
public:
    MyChart();

    void addPicObject();
    void addTextObject();
    void initObject();
    void EditData();
    void initPanel();
    void initData();

    int32_t showFormula(const SkString &formula_name, const formula::TaParam &param);
    int32_t addFormula(HPANEL panel, const SkString &formula_name, const formula::TaParam &param);

    // 添加一个序列
    void addDemoSeries(HPANEL panel, SeriesType typ);

private:
    HPANEL main_;
    HPANEL vol_ = nullptr;
    HPANEL pline_ = nullptr;
    HPANEL formula_ = nullptr;
    int32_t formula_id_ = 0;

    std::vector<chart_key_t> tms_;
};

#endif // MYCHART_H
