#include "contex.h"
#include "private.h"
#include "graphy/scene.h"

#ifdef SIA_OS_WIN

#include <windows.h>

namespace sia {
namespace rastery {

struct FramePrivate {
    BITMAPINFO *bmp_ = nullptr;

    void draw(HWND hwnd, SkPoint pt) {
        if (!bmp_) {
            return;
        }

        // 从image中提取有效的数据
        const int32_t w = bmp_->bmiHeader.biWidth;
        const int32_t h = -(bmp_->bmiHeader.biHeight);

        RECT rc;
        GetClientRect(hwnd, &rc);

//         std::cout << "paint window:" << (rc.right - rc.left) << "|" << w
//                   << "|" << (rc.bottom - rc.top) << "|" << h << std::endl;

        HDC dc = GetDC(hwnd);
        StretchDIBits(dc, 0, 0, w, h, pt.x(), pt.y(), w, h, bmp_->bmiColors, bmp_, DIB_RGB_COLORS, SRCCOPY);
        ReleaseDC(hwnd, dc);
    }
};
IMPLEMENT_FRAME_PRIVATE;

void Frame::set(BITMAPINFO* p) {
    impl_->bmp_ = p;
}

BITMAPINFO* Frame::get() {
    return impl_->bmp_;
}

void Frame::draw(HWND hwnd, SkPoint pt) {
    impl_->draw(hwnd, pt);
}

class ContextImpl : public Context {
public:
    virtual bool snapShot(Frame& f) override {
        BITMAPINFO* bmpInfo = reinterpret_cast<BITMAPINFO*>(mem_.get());
        f.set(bmpInfo);
        return true;
    }

    virtual void onSizeChanged() override {
        const auto w = width();
        const auto h = height();
        std::cout << "size changed:" << w << "|" << h << std::endl;

        const size_t bmpSize = sizeof(BITMAPINFOHEADER) + w * h * sizeof(uint32_t);
        mem_.reset(bmpSize);
        BITMAPINFO* bmpInfo = reinterpret_cast<BITMAPINFO*>(mem_.get());
        ZeroMemory(bmpInfo, sizeof(BITMAPINFO));
        bmpInfo->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bmpInfo->bmiHeader.biWidth = w;
        bmpInfo->bmiHeader.biHeight = -h; // negative means top-down bitmap. Skia draws top-down.
        bmpInfo->bmiHeader.biPlanes = 1;
        bmpInfo->bmiHeader.biBitCount = 32;
        bmpInfo->bmiHeader.biCompression = BI_RGB;
        void* pixels = bmpInfo->bmiColors;

        SkImageInfo info = SkImageInfo::Make(w, h, params_.fColorType, kPremul_SkAlphaType, params_.fColorSpace);
        d_->setSurface(SkSurface::MakeRasterDirect(info, pixels, sizeof(uint32_t) * w));
    }

private:
    SkAutoMalloc mem_;
};

IMPLEMENT_CONTEXT_CREATE_RELEASE

}
}

#endif
