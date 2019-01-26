#pragma once

#include "siapi.h"

#ifdef STATIC_LIB
#  define RASTERY_API
#else
#  if defined(rastery_EXPORTS)
#    define RASTERY_API SIA_EXPORT
#  else
#    define RASTERY_API SIA_IMPORT
#  endif
#endif

#ifdef SIA_OS_WIN
#include <windows.h>
#endif
