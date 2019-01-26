#pragma once

#include "siapi.h"
#include "SkView.h"
#include "paintgroup.h"
#include "event.h"

// Widget 是可视化组件的最小单位
// 所有的widget都在Scene中管理
namespace sia {
namespace graphy {

class Layout;
class GRAPHY_API Widget :
    public SkView {

    friend class Scene;
public:
    Widget(const SkString& name = SkString(), Widget* parent = nullptr);
    virtual ~Widget();

    // 更改父亲节点
    void setParent(Widget* parent);

    inline Widget* parent()const {
        return parent_;
    }

    SkRect rect()const;

    /*! \brief 在父亲中容器的位置
     *
     */
    SkRect geometry()const;

    /*! \brief 在scene中的位置
     *
     */
    SkRect windowGeometry()const;

    /*! \brief 窗口坐标转换到widget坐标上
     *
     *    @param    pt 窗口（scene）坐标
     *    @return   widget坐标
     */
    SkPoint windowToClient(SkPoint pt) const;

    /*! \brief widget坐标转换为window（scene）坐标
     *
     *    @param    pt widget坐标
     *    @return   窗口（scene）坐标
     */
    SkPoint clientToWindow(SkPoint pt) const;

    inline SkPoint pos()const {
        return pos_;
    }

    void setGeometry(const SkRect& r);

    void moveTo(SkScalar x, SkScalar y);

    void setZIndex(int16_t);

    void addChild(Widget* w);

    void setLayout(Layout* l);

    /*! \brief 立即重新来一次布局
     *
     *    @param
     *    @return
     */
    void relayout();

    inline SkString name()const {
        return name_;
    }

    void setName(const SkString& name);

    void forEachChild(std::function<bool(Widget*)> onChild)const;

    std::vector<Widget*> children()const {
        return children_;
    }

    inline int16_t zindex()const {
        return z_index_;
    }

    /*! \brief 移除孩子
     *
     *    @param	ch 孩子节点
     *    @return
     */
    void removeChild(Widget* ch);

    Widget* root() const;

    /*! \brief 孩子zindex 变化的时候调用
     *
     */
    void onChildZIndexChange(Widget* w);

    /*! \brief 向所有的孩子节点广播事件
     *
     *    @param	evt	需要广播的事件
     *    @return
     */
    void broadcast(const SkEvent& evt);

protected:
    virtual void onDestroy();
    virtual void onSizeChange() override;
    virtual void onLayoutChange() {
    }

    virtual void onDraw(SkCanvas* canvas) override;

    // 根据位置找到其下的Widget
    virtual Widget* hitTest(int x, int y);

    // 被挂载到UI树上
    virtual void onAttach() { }

    virtual bool onKey(sia::KeyBoard key, sia::InputState state, uint32_t modifiers) {
        return false;
    }

    virtual bool onMouse(int x, int y, sia::InputState state, uint32_t modifiers) {
        return false;
    }

    virtual bool onMouseWheel(float delta, uint32_t modifiers) {
        return false;
    }

protected:
    // internals
    bool onMouse_internal(int x, int y, InputState state, uint32_t modifiers);

    bool onMouseWheel_internal(float delta, uint32_t modifiers);

private:
    // 将孩子按照z_index递增排序排序
    void sortChildren();

    // 是否存在孩子
    bool childExist(Widget* w)const;

private:
    // 左上角的位置，坐标总是相对于parent widget
    SkPoint pos_;

    // 父亲节点，可以为空
    Widget* parent_ = nullptr;

    // 布局管理
    Layout* layout_ = nullptr;

    // 孩子节点
    std::vector<Widget*> children_;

    // z 轴顺序
    int16_t z_index_ = 0;

    // 对象名
    SkString name_;
};

}
}
