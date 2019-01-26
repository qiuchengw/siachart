#pragma once

#include "sk_app/Window.h"
#include "siapi.h"

namespace sia {

namespace graphy {
class Scene;
class Widget;
class Layout;
}

namespace platy {
class WindowPrivate;
class SIA_API XApp;
class SIA_API XWindow : public sk_app::Window::Layer {
public:
    XWindow();
    ~XWindow();

    bool create();
    void show(bool b);
    void inval();
    void addWidget(sia::graphy::Widget* w);

    graphy::Scene* scene()const {
        return sce_;
    }

    void setLayout(sia::graphy::Layout* l);

protected:
    virtual void onCreated() {
    }

    virtual void onPaint(SkCanvas*) override;

    virtual void onResize(int width, int height) override;

    virtual bool onKey(sk_app::Window::Key key, sk_app::Window::InputState state, uint32_t modifiers) override;
    virtual bool onMouse(int x, int y, sk_app::Window::InputState state, uint32_t modifiers) override;
    virtual bool onMouseWheel(float delta, uint32_t modifiers) override;

private:
    sk_app::Window* w_ = nullptr;
    graphy::Scene* sce_ = nullptr;
};
}
}
