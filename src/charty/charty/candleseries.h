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
class CandleSeries : public Series {
public:
    CandleSeries(const SkString& series_name);

protected:
    virtual void onDraw(SkCanvas* canvas) override;
    virtual void preparePaints() override;

    SkPaint candleStickPaint(bool up, bool fill)const;

private:
    SkPaint up_pat_;// = nullptr;
    SkPaint down_pat_;// = nullptr;
    SkScalar bar_width_ = SkIntToScalar(0);
};

}
}
