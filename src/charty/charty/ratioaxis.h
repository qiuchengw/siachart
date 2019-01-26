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
class RatioAxis : public Axis {
public:
    RatioAxis(Panel* parent = nullptr);

protected:
    virtual void onDraw(SkCanvas* canvas) override;
    virtual void preparePaints() override;

private:
    SkPaint *bk_ = nullptr;
    SkPaint *text_ = nullptr;
    SkPaint *above_text_ = nullptr;
    SkPaint *below_text_ = nullptr;
    SkPaint *border_ = nullptr;
};
}
}
