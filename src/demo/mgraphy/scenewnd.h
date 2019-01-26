#pragma once

#include "cwnd.h"
#include "graphy/scene.h"
#include "mode/rastery/contex.h"
#include "../mgraphy/widgets.h"

using namespace sia;

class SceneWnd : public CWindow {
public:
    ~SceneWnd() {
        sia::rastery::Context::release(ctx_);
    }

    virtual void onPaint() override {
        sia::rastery::Frame f;
        if (ctx_->doPaint(f)) {
            // 画到界面上
            f.draw(hWnd());
        }
    }

    virtual sia::graphy::Scene* createScene() {
        return nullptr;
    }

    virtual void onCreated() override {
        ctx_ = sia::rastery::Context::create();
        ctx_->setScene(createScene());
    }

    virtual void onResize(uint16_t w, uint16_t h) override {
        ctx_->setSize(w, h);
    }

    virtual bool onMouse(uint16_t x, uint16_t y, sia::InputState state, uint32_t mod) override {
        bool ret = ctx_->onMouse(x, y, state, mod);
        onPaint();
        return ret;
    }

    virtual bool onKey(sia::KeyBoard k, sia::InputState state, uint32_t mod) override {
        bool ret = ctx_->onKey(k, state, mod);
        onPaint();
        return ret;
    }

private:
    sia::rastery::Context* ctx_ = nullptr;
};


