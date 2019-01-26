#include "widget.h"
#include "layout.h"

namespace sia {
namespace graphy {

Widget::Widget(const SkString& name/* = ""*/, Widget* parent /*= nullptr*/) {
    // 一定要设置好，否则debug模式会导致layout计算不正确
    pos_.set(0, 0);

    setName(name);
    setParent(parent);
}

Widget::~Widget() {
    // 先移除掉所有的孩子节点
    for (auto ch : children_) {
        ch->setParent(nullptr);
    }
    children_.clear();

    // 再从父亲节点中移除
    if (Widget* p = parent()) {
        p->removeChild(this);
    }
}

void Widget::addChild(Widget* w) {
    if (!w || childExist(w)) {
        return;
    }

    children_.push_back(w);
    w->setParent(this);
    w->onAttach();
    sortChildren();

    relayout();
}

void Widget::setLayout(Layout* l) {
    layout_ = l;
    relayout();
}

void Widget::relayout() {
    if (layout_) {
        if (layout_->decoratedWidget() != this) {
            // 这个函数内部已经调用了doLayout了
            layout_->setDecoratedWidget(this);
        } else {
            layout_->doLayout();
        }
        onLayoutChange();
    }
}

void Widget::setName(const SkString& name) {
    name_ = name;
}

void Widget::forEachChild(std::function<bool(Widget*)> onChild) const {
    if (!onChild) {
        return;
    }

    for (Widget* w : children_) {
        if (!onChild(w)) {
            break;
        }
    }
}

void Widget::removeChild(Widget* ch) {
    if (!ch) {
        return;
    }

    for (auto i = children_.begin(); i != children_.end(); ++i) {
        if (*i == ch) {
            children_.erase(i);
            ch->setParent(nullptr);
            relayout();
            break;
        }
    }
}

Widget* Widget::root() const {
    Widget *p = const_cast<Widget*>(this);
    while (true) {
        if (!p->parent()) {
            return p;
        }
        p = p->parent();
    }
    return nullptr;
}

void Widget::onChildZIndexChange(Widget* w) {
    sortChildren();
}

void Widget::broadcast(const SkEvent& evt) {
    doEvent(evt);

    forEachChild([&](Widget* c)->bool {
        c->broadcast(evt);
        return true;
    });
}

void Widget::setParent(Widget* pa) {
    if (parent_ == pa) {
        return;
    }

    // 通知原来的父亲节点删除孩子
    if (parent_ && parent_->childExist(this)) {
        parent_->removeChild(this);
    }

    if (pa) {
        // 通知现在的父亲添加新的孩子
        pa->addChild(this);
    }

    // 记录自己的父亲节点
    parent_ = pa;
}

SkRect Widget::rect() const {
    return SkRect::MakeXYWH(0, 0, width(), height());
}

SkRect Widget::geometry() const {
    return SkRect::MakeXYWH(pos_.x(), pos_.y(), width(), height());
}

SkRect Widget::windowGeometry() const {
    SkPoint pt = clientToWindow(SkPoint::Make(0, 0));
    return SkRect::MakeXYWH(pt.x(), pt.y(), width(), height());
}

SkPoint Widget::windowToClient(SkPoint pt) const {
    const Widget* p = this;
    do {
        SkPoint of = p->pos();
        pt.offset(-of.x(), -of.y());

        // 一直找到最上层（我们认为最上层就是window）！
        p = p->parent();
    } while (p);
    return pt;
}

SkPoint Widget::clientToWindow(SkPoint pt) const {
    const Widget* p = this;
    do {
        SkPoint of = p->pos();
        pt.offset(of.x(), of.y());

        p = p->parent();
    } while (p);
    return pt;
}

void Widget::setGeometry(const SkRect& r) {
    moveTo(r.x(), r.y());
    setSize(r.width(), r.height());
}

void Widget::moveTo(SkScalar x, SkScalar y) {
    pos_.fX = x;
    pos_.fY = y;
}

void Widget::setZIndex(int16_t index) {
    z_index_ = index;

    if (parent_) {
        parent_->onChildZIndexChange(this);
    }
}

void Widget::onDestroy() {
    if (parent_) {
// 		SkEvent evt;
// 		parent_->doEvent(evt);
    }
}

void Widget::onSizeChange() {
    relayout();
}

void Widget::onDraw(SkCanvas* canvas) {
    for (Widget* c : children_) {
        // 我们要在这儿做好偏移的计算
        c->draw(canvas, c->pos());
    }
    SkView::onDraw(canvas);
}

Widget* Widget::hitTest(int x, int y) {
    // 先简单实现一个，使用遍历的方法
    // TODO: 使用BSP 树查找
    // 孩子按照zindex从小到大的顺序排序，所以后面的在最上面，最优先处理消息
    Widget* target = nullptr;
    for (auto i = children_.rbegin(); i != children_.rend(); ++i) {
        target = *i;
        if (target->geometry().contains(SkIntToScalar(x), SkIntToScalar(y))) {
            return target;
        }
    }
    return nullptr;
}

bool Widget::onMouse_internal(int x, int y, InputState state, uint32_t modifiers) {
    if (onMouse(x, y, state, modifiers)) {
        return true;
    }

    // 如果自己不处理，那么就给孩子们处理！
    if (Widget* w = hitTest(x, y)) {
        SkPoint pt = w->pos();
        return w->onMouse_internal(x - SkScalarFloorToInt(pt.x()), y - SkScalarFloorToInt(pt.y()), state, modifiers);
    }
    return false;
}

bool Widget::onMouseWheel_internal(float delta, uint32_t modifiers) {
    if (onMouseWheel(delta, modifiers)) {
        return true;
    }

    // 如果自己不处理，那么就给孩子们处理！
//     if (Widget* w = hitTest(x, y)) {
//         SkRect rc = w->geometry();
//         return w->onMouseWheel_internal(delta, modifiers);
//     }
    return false;
}

void Widget::sortChildren() {
    std::stable_sort(children_.begin(), children_.end(), [](Widget* w1, Widget* w2)->bool {
        return w1->zindex() < w2->zindex();
    });
}

bool Widget::childExist(Widget* w) const {
    return std::find(children_.begin(), children_.end(), w) != children_.end();
}

}
}
