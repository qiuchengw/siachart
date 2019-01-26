#pragma once

#include "defines.h"
#include "SkString.h"
#include "graphy/SkEvent.h"
#include "deps/util.h"

namespace sia {
namespace chartbase {
class DataSource;
}
namespace charty {

class Chart;

/*!
 * \class ChartContext
 *
 * \brief 图表内容数据
 *
 */
class ChartContext {
    friend class Chart;
public:
    ChartContext();

    int32_t curIndexCount();

    inline int32_t totalIndexCount()const {
        return ds_.total();
    }

    inline int32_t indexBegin()const {
        return index_begin_;
    }

    inline int32_t indexEnd()const {
        return index_end_;
    }

    void setCurrentDomain(int32_t b, int32_t e);

    inline const chartbase::DataSource* datasource() {
        return &ds_;
    }

    void addData(const SkString& data_name, const std::map<chart_key_t, chart_val_t>& datas);

    void removeData(const SkString& data_name);
    void removeData(const SkString& data_name, const SkTArray<chart_key_t>& keys);

    void emitEvent(const SkEvent& evt);

    void setSymbol(const SkString& sym);

    inline SkString symbol()const {
        return symbol_;
    }

    inline void setCalendarFormat(const SkString& format, const SkString& crosshair_calendar_format) {
        calendar_format_ = format;
        crosshair_calendar_format_ = crosshair_calendar_format;
    }

    const SkString& calendarFormat()const {
        return calendar_format_;
    }

    enum CalendarFormatType {
        kCalendarAxis = 0,
        kCalendarCrossHair,
    };
    SkString formatCalendar(chart_key_t d, CalendarFormatType typ = CalendarFormatType::kCalendarAxis)const;
    SkString formatCalendar(chart_key_t d, const SkString& format)const;

    bool functionEnabled(int32_t functions) const;

    void setCurVisualIndex(int idx) {
        cur_visual_index_ = idx;
    }

    int curVisualIndex()const {
        return cur_visual_index_;
    }

    /*! \brief 遍历图表上的panel
     *
     *    @param    panel_cb    回调，返回false停止遍历
     *    @return
     */
    void foreachPanel(std::function<bool(HPANEL)> panel_cb);

private:
    void setChart(Chart* c);

private:
    // 数据源
    chartbase::DataSource ds_;

    //! 当前十字线所在的visual index
    int cur_visual_index_ = -1;

    // 当前显示的序列 区间：[index_begin_, index_end_]
    int32_t index_begin_ = 0;
    int32_t index_end_ = 0;
    Chart* chart_ = nullptr;

    // 名字
    SkString symbol_;

    //! 时间/日期格式，参考cpp/strftime
    SkString calendar_format_;
    SkString crosshair_calendar_format_;

};
}
}
