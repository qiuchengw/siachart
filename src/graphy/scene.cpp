#include "scene.h"
#include <cassert>

namespace sia {
namespace graphy {

// void drawTooltip(SkCanvas* canvas, const SkString& text, const SkRect& rc) {
//     SkPaint p1;
//     p1.setColor(SkColorSetARGB(0xAA, 0xff, 0xff, 0xff));
//     canvas->drawRect(rc, p1);
//
//     SkPaint p2;
//     p2.setColor(SK_ColorBLACK);
//     canvas->drawString(text, rc.left(), rc.top() + 10, p2);
// }

Scene::Scene(const SkString& name /*= SkString()*/)
    :Widget(name) {

}

void Scene::paint(SkCanvas* canvas) {
    // canvas->clear(SK_ColorWHITE);
    if (!canvas) {
        return;
    }

    // 画自己的孩子节点
    draw(canvas);

    // 最后在所有的层之上，绘制Tooltip
    // drawTooltip(canvas, SkString(u8"Hello 中文"), SkRect::MakeXYWH(_mouse_x, _mouse_y, 100, 100));
    if (tooltip_ && !(tooltip_->parent())) {
        tooltip_->draw(canvas, tooltip_->pos());
    }
}

bool Scene::onMouse_internal(int x, int y, InputState state, uint32_t modifiers) {
    bool handled = false;
    SkPoint pt = SkPoint::Make(SkIntToScalar(x), SkIntToScalar(y));
    switch (state) {
    case InputState::kDown_InputState:
        tmd_.record(pt);
        if (tmd_.totalDuration() >= clickDuration()) {
            tmd_.reset();
            tmd_.record(pt);
        }
        break;

    case InputState::kUp_InputState:
        // 记录
        tmd_.record(pt);

        // 先发一个up消息
        handled = Widget::onMouse_internal(x, y, state, modifiers);

        // 看看是否需要发一个click消息
        if (tmd_.recordCount() == 2) {
            this->onMouse_internal(x, y, InputState::kClick_InputState, modifiers);
        }

        // 再看看是不是需要发一个double click消息
        if (tmd_.recordCount() >= 4) {
            if (tmd_.totalDuration() <= clickDuration()) {
                this->onMouse_internal(x, y, InputState::kDoubleClick_InputState, modifiers);
            } else {
                // 如果发double click，那么就要发一个单击消息
                this->onMouse_internal(x, y, InputState::kClick_InputState, modifiers);
            }
            tmd_.reset();
        }
        break;
    }

    if (state != InputState::kUp_InputState) {
        return Widget::onMouse_internal(x, y, state, modifiers);
    }
    // scene 可以被认为是最上层，我们认为只要消息到这儿，就是一定被处理过了。不用再向下传递了！
    return handled;
}

bool Scene::onMouseWheel_internal(float delta, uint32_t modifiers) {
    return Widget::onMouseWheel_internal(delta, modifiers);
}

bool Scene::onKey(sia::KeyBoard k, InputState state, uint32_t modif) {
    return Widget::onKey(k, state, modif);
}

void Scene::requestTooltip(Widget* tip) {
    if (tip->parent()) {
        // tooltip禁止设置父亲！
        assert(false);
        return;
    }

    tooltip_ = tip;
}

void Scene::hideTooltip() {
    tooltip_ = nullptr;
}

void Scene::setSurface(sk_sp<SkSurface> surf) {
    surf_ = surf;

    onSurfaceChanged();
}

}
}