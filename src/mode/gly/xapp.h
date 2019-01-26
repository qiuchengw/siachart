#pragma once

#include "sk_app/Application.h"
#include "sk_app/Window.h"
#include "XWindow.h"

// 全局的app变量
#define xApp ((sia::platy::XApp*)_s_xapp)

// paintman 用于从配置文件解析出来paint对象
// 可用于实现配置式的换肤

namespace sia {
namespace graphy {
class PaintMan;
}
namespace platy {

extern sia::platy::XApp* _s_xapp;

class SIA_API XWindow;
class SIA_API XApp : public sk_app::Application {
public:
    XApp();
    ~XApp();

    void setMainWindow(XWindow* w);

    XWindow* mainWindow()const {
        return mainwin_;
    }

    sk_app::Window::BackendType backendType()const {
        return backend_;
    }

    virtual void onIdle() override;

    void loadPaintConfig(const SkString& cfgpath);
protected:
    virtual void doCreate(int argc, char** argv, void* platformData) {
    }

private:
    XWindow* mainwin_ = nullptr;
    sk_app::Window::BackendType backend_ = sk_app::Window::kNativeGL_BackendType;
    my_sp<graphy::PaintMan> paintman_;
};
}
}
