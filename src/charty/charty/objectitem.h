#pragma once

#include "viewitem.h"
#include "viewobject.h"
#include "objectpos.h"

namespace sia {
namespace charty {

/*!
 * \class ObjectItem
 *
 * \brief 稀疏视图组件
 *
 */
class ObjectItem : public ViewItem {
public:
    ObjectItem(const SkString& name, MainView* parent);

    virtual ~ObjectItem();

    void setParam(ObjectParam para, my_sp<IVisualObject> obj);

    /*! \brief 此object是否依赖于其他的viewitem？
     *
     *    @param	item 是否依赖
     *    @return
     */
    bool isAttachedTo(ViewItem* item)const;

protected:
    virtual void onDraw(SkCanvas* canvas) override;

    virtual void onSizeChange() override;

    void reCalc();

    virtual void onSensed(int idx, const SkPoint& pt) override;

private:
    // 对象的位置信息
    ObjectParam para_;

    //! 真正的绘制对象
    my_sp<IVisualObject> obj_ = nullptr;

    //! 位置计算
    PositionCalc pos_calc_;

    //! 有效区
    SkRect object_visual_rc_;
};

}
}
