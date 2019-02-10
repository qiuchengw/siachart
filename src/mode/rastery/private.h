#pragma once

#include <memory>
#include "graphy/scene.h"

namespace sia {
namespace rastery {

#define IMPLEMENT_CONTEXT_CREATE_RELEASE \
Context* Context::create() { return new ContextImpl(); } \
bool Context::release(Context* ctx) { delete ctx; return true; }

#define IMPLEMENT_FRAME_PRIVATE \
Frame::Frame() { impl_.reset(new FramePrivate()); }

enum class ContextPrivateEvent {
    kSurfChanged,
};

typedef std::function<void(ContextPrivateEvent, void*)> CtxPrivateEventCallback;
struct ContextPrivate {
private:
    sk_sp<SkSurface> surf_;
    CtxPrivateEventCallback ccb_;

public:
    std::unique_ptr<graphy::Scene> scen_ = nullptr;

    ContextPrivate(CtxPrivateEventCallback cb)
        :ccb_(cb) {
        setScene(new graphy::Scene());
    }

    sk_sp<SkSurface> surface()const {
        return surf_;
    }

    void setSurface(sk_sp<SkSurface> surf) {
        surf_ = surf;

        if (scen_) {
            scen_->setSurface(surf);
        }

        if (ccb_) {
            ccb_(ContextPrivateEvent::kSurfChanged, nullptr);
        }
    }

    void setScene(graphy::Scene* s) {
        scen_.reset(s);
    }

    SkCanvas* canvas()const {
        if (surf_) {
            return surf_->getCanvas();
        }
        return nullptr;
    }

    SkScalar width()const {
        if (scen_) {
            return scen_->width();
        }
        return 0.f;
    }

    SkScalar height()const {
        if (scen_) {
            return scen_->height();
        }
        return 0.f;
    }

    sk_sp<SkImage> snapShot()const {
        if (surf_) {
            return surf_->makeImageSnapshot();
        }
        return nullptr;
    }

    void setSize(int w, int h) {
        if (scen_) {
            scen_->setSize(SkIntToScalar(w), SkIntToScalar(h));
        }
    }
};

}
}
