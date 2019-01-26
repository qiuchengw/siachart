#include "chartctx.h"
#include "chart.h"

namespace sia {

namespace charty {
ChartContext::ChartContext() {
    calendar_format_ = "%H:%M";
}

int32_t ChartContext::curIndexCount() {
    if (index_end_ < 0) {
        setCurrentDomain(index_begin_, index_end_);
    }
    return index_end_ - index_begin_ + 1;
}

void ChartContext::setCurrentDomain(int32_t b, int32_t e) {
    if (b <= 0) {
        b = 0;
    }

    if (e <= -1 || e >= totalIndexCount()) {
        e = totalIndexCount() - 1;
    }

    index_begin_ = b;
    index_end_ = e;

    emitEvent(SkEvent(ChartEventType::kDomainChanged));
}

void ChartContext::addData(const SkString& data_name, const std::map<chart_key_t, chart_val_t>& datas) {
    ds_.addData(data_name, datas);

    emitEvent(SkEvent(ChartEventType::kDataChanged));
}

void ChartContext::removeData(const SkString& data_name) {
    ds_.removeData(data_name);
    emitEvent(SkEvent(ChartEventType::kDataChanged));
}

void ChartContext::removeData(const SkString& data_name, const SkTArray<chart_key_t>& keys) {
    ds_.removeData(data_name, keys);
    emitEvent(SkEvent(ChartEventType::kDataChanged));
}

void ChartContext::emitEvent(const SkEvent& evt) {
    if (chart_) {
        chart_->doEvent(evt);
    }
}

void ChartContext::setSymbol(const SkString& sym) {
    symbol_ = sym;
}

SkString ChartContext::formatCalendar(chart_key_t d, CalendarFormatType typ /*= CalendarFormatType::kCalendarAxis*/) const {
    SkString formater;
    switch (typ) {
    case CalendarFormatType::kCalendarAxis:
        formater = calendar_format_;
        break;

    case CalendarFormatType::kCalendarCrossHair:
        formater = crosshair_calendar_format_;
        break;

    default:
        return SkString("--");
    }
    return formatCalendar(d, formater);
}

SkString ChartContext::formatCalendar(chart_key_t d, const SkString& format) const {
    std::tm dt;
    siaParseChartKey(d, dt);

    char buf[100] = { 0 };
    auto n = std::strftime(buf, 100, format.c_str(), &dt);
    return SkString(buf, n);
}

void ChartContext::setChart(Chart* c) {
    chart_ = c;
}

bool ChartContext::functionEnabled(int32_t functions) const {
    if (chart_) {
        return chart_->functionEnabled(functions);
    }
    return false;
}

void ChartContext::foreachPanel(std::function<bool(HPANEL)> panel_cb) {
    if (!chart_ || !panel_cb) {
        return;
    }

    for (uint32_t i = 0; i < chart_->panelCount(); ++i) {
        if (!panel_cb(chart_->panel(i))) {
            break;
        }
    }
}

}

}

