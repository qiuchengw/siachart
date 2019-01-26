#pragma once

#include "visualitem.h"
#include "validarea.h"

namespace sia {
namespace charty {

class MainView;
class Panel;

/*!
 * \class SensorWidget
 *
 * \brief 能够感知鼠标悬停的控件，必须放在MainView中，且必须完全覆盖MainView才可以！
 *
 */
class SensorWidget : public VisualItem {
    friend class MainView;
    enum _SensorFlags {
        //! 是否正在感应区中
        kInSensing = 0x1,

        // 是否已提示感应
        kSensorActived = 0x2,
    };

public:
    SensorWidget(const SkString& name, MainView* parent);

    inline void resetSensorAreas(int n) {
        area_.reset(n);
    }

    inline void setSensorArea(int idx, const SkRect& rc) {
        area_.set(idx, rc);
    }

    inline void setSensorArea(int idx, const SkPoint& pt_center, SkScalar width, SkScalar height) {
        auto rc = SkRect::MakeXYWH(pt_center.x() - width / 2, pt_center.y() - height / 2, width, height);
        setSensorArea(idx, rc);
    }

    inline const SkRect& sensorArea(int idx)const {
        return area_.get(idx);
    }

    inline void setRegion(const SkRegion& pa) {
        r1_ = pa;
    }

    inline void setOnePieceSensor(const SkRect& rc) {
        area_.setOne(rc);
    }

    inline bool inSensorArea(int idx, const SkPoint& pt) const {
        // return r1_.contains(pt.x(), pt.y());
        return area_.contains(idx, pt);
    }

    /*! \brief 每个index占用的宽度
     *
     *    @return
     */
    inline SkScalar indexWidth()const {
        return index_width_;
    }

    inline void setIndexWidth(SkScalar w) {
        index_width_ = w;
    }

    MainView* mainView() const;

    Panel* panel() const;

    void forEachSensor(std::function<bool(int, const SkRect&)> func)const;

    inline int sensorCount()const {
        return area_.count();
    }

protected:
    virtual bool onMouse(int x, int y, sia::InputState state, uint32_t modifiers) override;

    /*! \brief 当鼠标进入感应区内的时候会调用此消息
     *
     *    @param
     *    @return
     */
    virtual void onSensed(int idx, const SkPoint& pt) {

    }

    /*! \brief 在感应区内点击
     *
     *    @param
     *    @return
     */
    virtual void onClickSensor(int idx, const SkPoint& pt) {

    }

private:
    //! 整个视图区宽度被等分，每一份占用的宽度大小
    SkScalar index_width_ = 1.f;

    // 按index记录有效区
    ValidArea area_;

    // 是否已经提示感应了
    // BitFlags sflags_;
    SkRegion r1_;
};

}
}
