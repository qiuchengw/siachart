#pragma once

#include <QImage>
#include <QPixmap>
#include "deps/os.h"

#ifdef SIA_OS_LINUX
struct _XImage;
#elif defined(SIA_OS_WIN)
#include <Windows.h>
#endif

namespace sia {
namespace port {

#ifdef SIA_OS_WIN

namespace QtWin {
// decode bitmap into QPixmap
QPixmap fromBITMAPINFO(BITMAPINFO *bitmap);

} // QtWin
#endif

#ifdef SIA_OS_LINUX
namespace QtLinux {

// decode XImage into QImage
QImage fromXImage(::_XImage* xi);

} // QtLinux
#endif

}   // port
}// sia
