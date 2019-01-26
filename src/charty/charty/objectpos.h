#pragma once

#include "viewobject.h"

namespace sia {
namespace charty {
class MainView;
class ViewItem;

/*!
 * \class ObjectPosCalc
 *
 * \brief 对象坐标计算
 *
 *
 */
class PositionCalc {
public:
    /*! \brief 计算出对象合适的geometry，
     *	    仅支持相对父容器的计算
     *
     *    @param    view    容器
     *    @param    sz  期望大小，可以传入空值，让程序自己判断合适大小
     *    @param    object_rect  计算出的geometry
     *
     *    @return 计算错误，或者位置超出view显示范围，返回false
     */
    bool calc(MainView* view, SkSize sz, SkRect& object_rect);

    void setRelatePos(const RelPos& p) {
        pos_ = PosType::kRelated;
        rel_ = p;
    }

    void setAttached(ViewItem* item, chart_key_t x);

    void setDataValue(chart_val_t val) {
        pos_ = PosType::kValued;
        val_ = val;
    }

    /*! \brief 参考item_point_/item_rc_的注释！
     *
     *    @param    rc [out]
     *    @param    pt [out]
     *    @return
     */
    void getAttachedItem(SkRect& rc, SkPoint& attach_pt, SkPoint& object_pt) {
        rc = item_rc_;
        attach_pt = item_point_;
        object_pt = object_point_;
    }

    inline ViewItem* attachedView()const {
        return attach_;
    }

protected:
    // 相对位置
    SkRect calcRelate();

    /*! \brief 计算attach矩形
     *
     *    @return   object的位置
     */
    SkRect calcAttach();

    // 值
    SkRect calcDataVal();

    bool validCoord(SkScalar coo) const;

private:
    PosType pos_ = PosType::kNotSet;

    //! 期望大小
    SkSize sz_;

    //! 容器
    MainView* view_ = nullptr;

    //! 相对坐标
    RelPos rel_;

    //! attach到此组件
    ViewItem* attach_ = nullptr;

    //! x坐标的位置（attach的时候有效）
    chart_key_t x_ = chartbase::kInvalidDataKey;

    //! 根据value计算
    chart_val_t val_ = chartbase::kInvalidDataValue;

    // 计算输出
    //! 标注的话，要画一条直线，以让标注更清楚，要知道从哪个地方开始画线的起点
    //  起点就是被标注的item的sensor rect合适的角点
    // 只有PosType::kAttached的时候有效
    SkPoint item_point_;

    // 计算出的attached object 合适的标注线，结束点
    SkPoint object_point_;

    //! 被标注的item的rect（如 bar / line / candle的在当前key上的位置）
    // 只有PosType::kAttached的时候有效
    SkRect item_rc_;
};

}
}
