#pragma once

#include "siapi.h"
#include "widget.h"
#include "deps/util.h"
#include "SkSurface.h"

// Scene 用于管理基本的组件试图
// 并和系统Surface/Window做交互
namespace sia {
namespace graphy {

class Tooltip;
class GRAPHY_API Scene : public Widget {
public:
    Scene(const SkString& name = SkString());

    virtual void paint(SkCanvas* canvas);

    // 覆盖掉widget的同名函数，以便对外提供公用的函数
    bool onMouse_internal(int x, int y, InputState state, uint32_t modifiers);
    bool onMouseWheel_internal(float delta, uint32_t modifiers);

    bool onKey(sia::KeyBoard k, InputState state, uint32_t modif);

    /*! \brief 两次连续点击间隔少于此值，可以认为是双击
     *
     *    @return
     */
    inline int32_t clickDuration()const {
        return 500; // msec
    }

    /*! \brief 请求显示一个Tooltip
     *	     scene将在下一个绘制周期的最后阶段绘制这个Tooltip，以确保它在所有其他元素之上。
     *       绘制的位置需要实现设置好Tooltip的geometry（以Scene坐标为参照）
     *    @param
     *    @return
     */
    void requestTooltip(Widget* tip);

    /*! \brief 隐藏tooltip
     *
     *    @param
     *    @return
     */
    void hideTooltip();

    void setSurface(sk_sp<SkSurface> surf);

    sk_sp<SkSurface> surface()const {
        return surf_;
    }

protected:
    virtual void onSurfaceChanged() {};

private:
    sk_sp<SkSurface> surf_;

    util::StopWatch<SkPoint> tmd_;
    Widget* tooltip_ = nullptr;
};
}
}
