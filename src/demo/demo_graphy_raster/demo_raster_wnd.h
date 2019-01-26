#pragma once

#include "cwnd.h"
#include "graphy/scene.h"
#include "mode/rastery/contex.h"
#include "../mgraphy/widgets.h"
#include "../mgraphy/scenewnd.h"

using namespace sia;
class MyScene : public sia::graphy::Scene {
public:
    virtual bool onMouse(int x, int y, sia::InputState state, uint32_t modifiers) override {
        switch (state) {
        case InputState::kDown_InputState:
            pressed_ = true;
            pt_lt_.set(x, y);
            pt_rb_ = pt_lt_;
            break;

        case InputState::kMove_InputState:
            if (pressed_) {
                pt_rb_.set(x, y);
            }
            break;

        case InputState::kUp_InputState:
            pressed_ = false;
            break;
        }
        return true;
    }

protected:
    virtual void paint(SkCanvas* canvas) override {
        if (!canvas) {
            return;
        }

        sia::graphy::Scene::paint(canvas);

        SkPaint p1;
        p1.setColor(SkColorSetARGB(0xAA, 3, 121, 215));

        // 画band
        if (pressed_) {
            SkRect rc = SkRect::MakeLTRB(pt_lt_.x(), pt_lt_.y(), pt_rb_.x(), pt_rb_.y());
            canvas->drawRect(rc, p1);
        }

        // 画十字线
        canvas->drawLine(0, pt_rb_.y(), width(), pt_rb_.y(), p1); // 水平
        canvas->drawLine(pt_rb_.x(), 0, pt_rb_.x(), height(), p1); // 垂直
    }

private:
    bool pressed_ = false;
    SkPoint pt_lt_;
    SkPoint pt_rb_;
};

class DemoRasterWnd : public SceneWnd {
public:
    ~DemoRasterWnd() {
        sia::rastery::Context::release(ctx_);
    }

    virtual sia::graphy::Scene* createScene() override {
        auto *s = new MyScene();
        test::create_widgets(s);
        return s;
    }

private:
    sia::rastery::Context* ctx_ = nullptr;
};

CWindow* create_wnd(HINSTANCE inst) {
    return new DemoRasterWnd;
}

