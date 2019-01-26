#pragma once

#include "SkImageInfo.h"
#include "SkSurfaceProps.h"
#include "SkAutoMalloc.h"
#include "SkSurface.h"
#include "rasteryapi.h"
#include "graphy/event.h"

#ifdef SIA_OS_WIN
#include <windows.h>
#elif defined(SIA_OS_LINUX)
// #include <X11/Xlib.h>
struct _XImage;
struct _XDisplay;
struct _XGC;
#endif

// 提供surface/canvas/pixmap的接口
namespace sia {

namespace graphy {
class Scene;
class PaintMan;
}

namespace rastery {

struct DisplayParams {
    DisplayParams()
        : fColorType(kN32_SkColorType)
        , fColorSpace(nullptr)
        , fMSAASampleCount(1)
        , fSurfaceProps(SkSurfaceProps::kLegacyFontHost_InitType)
    {}

    SkColorType         fColorType;
    sk_sp<SkColorSpace> fColorSpace;
    int                 fMSAASampleCount;
    SkSurfaceProps      fSurfaceProps;
};

struct FramePrivate;
// 本来可以通过集成一个SkImage统一所有平台的接口。
// 但是这样却导致图像数据做多次转换，降低运行效率。
// 所以我们此处直接使用平台原生的图像类型，以提升绘图效率
class RASTERY_API Frame {
public:
    Frame();
    ~Frame();

#ifdef SIA_OS_WIN
    void set(BITMAPINFO* p);
    BITMAPINFO* get();
    void draw(HWND hwnd, SkPoint pt = SkPoint::Make(0, 0));
#elif defined(SIA_OS_LINUX)
    _XImage* get();
    void draw(_XDisplay* display, unsigned long window, _XGC* gc, SkPoint pt = SkPoint::Make(0, 0));
#endif

    inline my_sp<FramePrivate> impl() {
        return impl_;
    }

protected:
    my_sp<FramePrivate> impl_ = nullptr;
};

struct ContextPrivate;
class RASTERY_API Context {
protected:
    Context();
    virtual ~Context();

public:
    static Context* create();
    static bool release(Context* ctx);

    // 绘图接口
    SkCanvas* canvas();

    // 大小变化
    void setSize(int w, int h);

    graphy::Scene* scene()const;

    // 设置自定制的Scene，对象必须是new出来的！
    void setScene(graphy::Scene* s)const;

    SkScalar width()const;

    SkScalar height()const;

    bool doPaint(Frame& f);

    sk_sp<SkImage> snapShot()const;

public:
    // 绘制的内容是什么？
    virtual bool snapShot(Frame& f);

    bool onKey(sia::KeyBoard k, sia::InputState state, uint32_t modifiers);

    bool onMouse(int x, int y, sia::InputState state, uint32_t modifiers);
    bool onMouseWheel(float delta, uint32_t modifiers);

protected:
    virtual void onSizeChanged();

protected:
    DisplayParams params_;
    std::unique_ptr<ContextPrivate> d_;
};

}
}
