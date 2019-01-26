#pragma once

#include "series.h"

namespace sia {
namespace charty {
/*!
 * \class MacdSeries
 *
 * \brief ªÊ÷∆macdœﬂ
 *
 *
 */
class UpdownSeries : public Series {
public:
    UpdownSeries(const SkString& series_name);

protected:
    virtual void onDraw(SkCanvas* canvas) override;
    virtual void preparePaints() override;

private:
    SkPaint up_pat_;
    SkPaint down_pat_;
    SkScalar bar_width_ = SkIntToScalar(1);
};


}
}
