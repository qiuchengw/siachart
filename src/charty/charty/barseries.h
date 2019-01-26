#pragma once

#include "series.h"

namespace sia {
namespace charty {
/*!
 * \class Series
 *
 * \brief 序列用于展示图表的数据内容
 *
 */
class BarSeries : public Series {
public:
    BarSeries(const SkString& series_name);

protected:
    virtual void onDraw(SkCanvas* canvas) override;
    virtual void preparePaints() override;

private:
    SkPaint pat_;
};

}
}
