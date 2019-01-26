#pragma once

#include "sensorwidget.h"

namespace sia {
namespace charty {

enum class ViewItemType {
    kNotSet,
    kViewSeries,
    kViewObject,
};

/*!
 * \class ViewItem
 *
 * \brief 完全覆盖在MainView上的视图
 *
 *
 */
class ViewItem : public SensorWidget {
protected:
    enum ViewItemFlag {
        kSelected = 1 << 1,

        // 此处最大可用的flag个数
        kMaxFlagShift = 8,
    };
public:
    ViewItem(ViewItemType typ, const SkString& name, MainView* parent);

    virtual ~ViewItem();

    SkScalar contentWidth()const;

    SkScalar contentHeight()const;

    inline ViewItemType viewType()const {
        return vtype_;
    }

    /*! \brief 获取key序列当前在View中的有效可视区域（目前用的是Sensor区）
     *
     *    @param    key 序列key
     *              rc_out sensor区
     *
     *    @return   如果key在当前序列中没有显示，则返回false
     */
    virtual bool rectOfKey(chart_key_t key, SkRect& rc_out) const;

    /*! \brief 选中Series
     *
     *    @param	sel		选中与否
     *    @return
     */
    void select(bool sel);

    inline bool selected()const;

protected:
    virtual void onDraw(SkCanvas* canvas) override;

    virtual void onClickSensor(int idx, const SkPoint& pt) override;

protected:
    BitFlags flags_;

private:
    ViewItemType vtype_ = ViewItemType::kNotSet;
};
}
}
