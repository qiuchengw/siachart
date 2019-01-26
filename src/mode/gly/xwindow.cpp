#include "xwindow.h"
#include <assert.h>
#include "sk_app/Window.h"
#include "sk_app/DisplayParams.h"
#include "xapp.h"
#include "graphy/scene.h"

namespace sia {
namespace platy {

XWindow::XWindow() {

}

XWindow::~XWindow() {
    w_->detach();
    delete w_;
}

bool XWindow::create() {
    if (w_) {
        assert(false);
        return true;
    }

    if (w_ = sk_app::Window::CreateNativeWindow(nullptr)) {
        w_->setRequestedDisplayParams(sk_app::DisplayParams());
        sce_ = new graphy::Scene();
        // sce_->setGeometry(SkRect::MakeXYWH(0, 0, ));
        onCreated();

        // register callbacks
        w_->pushLayer(this);
        w_->attach(xApp->backendType());
        return true;
    }
    return false;
}

void XWindow::show(bool b) {
    if (w_) {
        if (b) {
            w_->show();
            w_->inval();
        }
    }
}

void XWindow::inval() {
    if (w_) {
        w_->inval();
    }
}

void XWindow::addWidget(sia::graphy::Widget* w) {
    if (sce_) {
        sce_->addChild(w);
    }
}

void XWindow::setLayout(sia::graphy::Layout* l) {
    if (sce_) {
        sce_->setLayout(l);
    }
}

void XWindow::onPaint(SkCanvas* canvas) {
    // 让scence类代理绘制
    sce_->paint(canvas);
}

void XWindow::onResize(int width, int height) {
    sce_->setGeometry(SkRect::MakeXYWH(0, 0, SkIntToScalar(width), SkIntToScalar(height)));
}

bool XWindow::onKey(sk_app::Window::Key key, sk_app::Window::InputState state, uint32_t modifiers) {
    if (!sce_) {
        return false;
    }

    return false;
    // return sce_->onKey((int32_t)key, state, modifiers);
}

bool XWindow::onMouse(int x, int y, sk_app::Window::InputState state, uint32_t modifiers) {
    if (!sce_) {
        return false;
    }

    return sce_->onMouse_internal(x, y, state, modifiers);
}

bool XWindow::onMouseWheel(float delta, uint32_t modifiers) {
    if (!sce_) {
        return false;
    }

    return sce_->onMouseWheel_internal(delta, modifiers);
}

}
}
