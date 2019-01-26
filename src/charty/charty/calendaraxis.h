#pragma once

#include "axis.h"

namespace sia {
namespace charty {

/*!
 * \class VisualItem
 *
 * \brief 图表内所有可见的区域元素都要继承于此类
 *
 */
class CalendarAxis : public Axis {
public:
    CalendarAxis(Panel* parent);

protected:
    virtual void onDraw(SkCanvas* canvas) override;
    virtual void preparePaints() override;

private:
    SkPaint* bk_ = nullptr;
    SkPaint* text_ = nullptr;
    SkPaint* border_ = nullptr;

};
}
}
