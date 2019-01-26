#include "contex.h"
#include "private.h"
#include "graphy/scene.h"

#ifdef SIA_OS_LINUX

#include <X11/Xlib.h>
#include <GL/glx.h>

namespace sia {
    namespace rastery {

        struct FramePrivate {
            SkPixmap pm;
            XImage img;

            void draw(Display* display, Window window, GC gc, SkPoint pt) {
                // gc = XCreateGC(fDisplay, fWindow, 0, nullptr);
                XPutImage(display, window, gc, &img, 0, 0, pt.x(), pt.y(), pm.width(), pm.height());
            }
        };
        IMPLEMENT_FRAME_PRIVATE;

        XImage* Frame::get() {
            return &(impl_->img);
        }

        void Frame::draw(_XDisplay* display, unsigned long window, _XGC* gc, SkPoint pt) {
            impl_->draw(display, window, gc, pt);
        }

        // context
        class ContextImpl : public Context {
        public:
            virtual bool snapShot(Frame& f) override {
                auto imp = f.impl();
                if (!imp || !d_->surface()->peekPixels(&(imp->pm))) {
                    return false;
                }
                int bitsPerPixel = imp->pm.info().bytesPerPixel() * 8;

                // ×ª»»µ½ximage
                memset(&imp->img, 0, sizeof(imp->img));
                imp->img.width = imp->pm.width();
                imp->img.height = imp->pm.height();
                imp->img.format = ZPixmap;
                imp->img.data = (char*)imp->pm.addr();
                imp->img.byte_order = LSBFirst;
                imp->img.bitmap_unit = bitsPerPixel;
                imp->img.bitmap_bit_order = LSBFirst;
                imp->img.bitmap_pad = bitsPerPixel;
                imp->img.depth = 24;
                imp->img.bytes_per_line = imp->pm.rowBytes() - imp->pm.width() * imp->pm.info().bytesPerPixel();
                imp->img.bits_per_pixel = bitsPerPixel;
                return XInitImage(&imp->img);
            }

            virtual void onSizeChanged() override {
                const auto w = width();
                const auto h = height();

                SkImageInfo info = SkImageInfo::Make(w, h, params_.fColorType, kPremul_SkAlphaType, params_.fColorSpace);
                d_->setSurface(SkSurface::MakeRaster(info, &params_.fSurfaceProps));
            }
        };
        IMPLEMENT_CONTEXT_CREATE_RELEASE;

    }
}

#endif
