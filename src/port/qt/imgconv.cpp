#include "imgconv.h"

#ifdef SIA_OS_LINUX
#include <X11/Xlib.h>
#endif

namespace sia {
namespace port {

#ifdef SIA_OS_WIN

namespace QtWin {
static inline void copyImageDataCreateAlpha(const uchar *data, QImage *target) {
    const uint mask = target->format() == QImage::Format_RGB32 ? 0xff000000 : 0;
    const int height = target->height();
    const int width = target->width();
    const int bytesPerLine = width * int(sizeof(QRgb));
    for (int y = 0; y < height; ++y) {
        QRgb *dest = reinterpret_cast<QRgb *>(target->scanLine(y));
        const QRgb *src = reinterpret_cast<const QRgb *>(data + y * bytesPerLine);
        for (int x = 0; x < width; ++x) {
            const uint pixel = src[x];
            if ((pixel & 0xff000000) == 0 && (pixel & 0x00ffffff) != 0)
                dest[x] = pixel | 0xff000000;
            else
                dest[x] = pixel | mask;
        }
    }
}

QPixmap fromBITMAPINFO(BITMAPINFO *bitmap) {
    const int w = bitmap->bmiHeader.biWidth;
    const int h = -(bitmap->bmiHeader.biHeight);

    const QImage::Format imageFormat = QImage::Format_ARGB32_Premultiplied;
    // Create image and copy data into image.
    QImage image(w, h, imageFormat);
    if (image.isNull()) { // failed to alloc?
        return QPixmap();
    }
    copyImageDataCreateAlpha((const uchar*)bitmap->bmiColors, &image);
    return QPixmap::fromImage(image);
}

} // QtWin
#endif

#ifdef SIA_OS_LINUX

namespace QtLinux {

QImage fromXImage(XImage* xi) {
    QImage::Format format = QImage::Format_ARGB32_Premultiplied;
    if (xi->depth == 24)
        format = QImage::Format_RGB32;
    else if (xi->depth == 16)
        format = QImage::Format_RGB16;

    QImage image = QImage(reinterpret_cast<uchar*>(xi->data), xi->width, xi->height, xi->bytes_per_line, format).copy();

    // we may have to swap the byte order
    if (xi->byte_order == MSBFirst || xi->byte_order == LSBFirst) {

        for (int i = 0; i < image.height(); i++) {
            if (xi->depth == 16) {
                ushort* p = reinterpret_cast<ushort*>(image.scanLine(i));
                ushort* end = p + image.width();
                while (p < end) {
                    *p = ((*p << 8) & 0xff00) | ((*p >> 8) & 0x00ff);
                    p++;
                }
            } else {
                uint* p = reinterpret_cast<uint*>(image.scanLine(i));
                uint* end = p + image.width();
                while (p < end) {
                    *p = ((*p << 24) & 0xff000000) | ((*p << 8) & 0x00ff0000)
                         | ((*p >> 8) & 0x0000ff00) | ((*p >> 24) & 0x000000ff);
                    p++;
                }
            }
        }
    }

    // fix-up alpha channel
    if (format == QImage::Format_RGB32) {
        QRgb* p = reinterpret_cast<QRgb*>(image.bits());
        for (int y = 0; y < xi->height; ++y) {
            for (int x = 0; x < xi->width; ++x)
                p[x] |= 0xff000000;
            p += xi->bytes_per_line / 4;
        }
    }

    return image;
}

} // QtLinux
#endif

}   // port
}// sia
