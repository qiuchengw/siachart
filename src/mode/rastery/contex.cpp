#include "contex.h"
#include "private.h"
#include "graphy/scene.h"

namespace sia {
namespace rastery {

class ContextImpl;

Frame::~Frame() {

}

Context::Context() {
    auto ctxEvt = [](ContextPrivateEvent evt, void* d) {
    };
    d_.reset(new ContextPrivate(ctxEvt));
}

Context::~Context() {
}


SkCanvas* Context::canvas() {
    return d_->canvas();
}

SkScalar Context::width() const {
    return d_->width();
}

SkScalar Context::height() const {
    return d_->height();
}

void Context::setSize(int w, int h) {
    if (w > 0 && h > 0 && (w != width() || h != height())) {
        d_->setSize(w, h);
        onSizeChanged();
    }
}

bool Context::doPaint(Frame& f) {
    d_->scen_->paint(canvas());
    return snapShot(f);
}

bool Context::snapShot(Frame& f) {
//     sk_sp<SkImage> img = impl_->snapShot();
//     if (img) {
//         f.set(img);
//         return true;
//     }
    return false;
}

sk_sp<SkImage> Context::snapShot() const {
    return d_->snapShot();
}

bool Context::onKey(sia::KeyBoard k, sia::InputState state, uint32_t modifiers) {
    return d_->scen_->onKey(k, state, modifiers);
}

bool Context::onMouse(int x, int y, sia::InputState state, uint32_t modifiers) {
    return d_->scen_->onMouse_internal(x, y, state, modifiers);
}

bool Context::onMouseWheel(float delta, uint32_t modifiers) {
    return d_->scen_->onMouseWheel_internal(delta, modifiers);
}

void Context::onSizeChanged() {

}

sia::graphy::Scene* Context::scene() const {
    return d_->scen_.get();
}

void Context::setScene(graphy::Scene* s) const {
    if (!s) {
        return;
    }
    d_->setScene(s);
    s->setSurface(d_->surface());
}

}
}