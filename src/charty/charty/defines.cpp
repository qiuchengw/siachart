#include "defines.h"
#include "panel.h"
#include <cassert>

namespace sia {
namespace charty {
void siaParseChartKey(chart_key_t d, std::tm& dt) {
    // 20181008162430435 = 2018-10-08 16:24:30.435
    memset(&dt, 0, sizeof(dt));

    // https://en.cppreference.com/w/cpp/chrono/c/tm

    // millsecs
    d /= 1000;

    dt.tm_sec = int(d % 100);
    d /= 100;
    assert(dt.tm_sec >= 0 && dt.tm_sec <= 60);

    dt.tm_min = int(d % 100);
    d /= 100;
    assert(dt.tm_min >= 0 && dt.tm_min <= 59);

    dt.tm_hour = int(d % 100);
    d /= 100;
    assert(dt.tm_hour >= 0 && dt.tm_hour <= 23);

    dt.tm_mday = int(d % 100);
    d /= 100;
    assert(dt.tm_mday >= 1 && dt.tm_mday <= 31);

    // int tm_mon : months since January –[0, 11]
    dt.tm_mon = int(d % 100) - 1;
    if (dt.tm_mon < 0) {
        dt.tm_mon = 0;
    }
    assert(dt.tm_mon >= 0 && dt.tm_mon <= 11);
    d /= 100;

    // std::tm的年是从1900年开始累计的
    dt.tm_year = (int)d - 1900;
}


chart_key_t siaMakeChartKey(const std::tm& dt) {
    // https://en.cppreference.com/w/cpp/chrono/c/tm
    // int tm_mon : months since January –[0, 11]
    chart_key_t d = ((dt.tm_year * 100 + (dt.tm_mon + 1)) * 100) + dt.tm_mday;
    d = (((((d * 100 + dt.tm_hour) * 100) + dt.tm_min) * 100) + dt.tm_sec) * 1000;
    return d;
}

SkString ChartFormater::formatVal(chart_val_t val) const {
    if (Panel* p = ToPanel(panel_)) {
        auto &prec = p->mainView()->precision();
        return prec.chartValFormat(val);
    }
    return SkString("");
}

SkString ChartFormater::formatKey(chart_key_t key, const SkString& format) const {
    if (Panel* p = ToPanel(panel_)) {
        if (ChartContext* ctx = p->chartCtx()) {
            if (format.isEmpty()) {
                return ctx->formatCalendar(key, ChartContext::kCalendarAxis);
            } else {
                return ctx->formatCalendar(key, format);
            }
        }
    }
    return SkString("--");
}

void ChartFormater::setPanel(HPANEL panel) {
    panel_ = panel;
}

}
}
