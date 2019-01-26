#pragma once

#include "siapi.h"

#ifdef STATIC_LIB
#  define RESY_API
#else
#  if defined(resy_EXPORTS)
#    define RESY_API SIA_EXPORT
#  else
#    define RESY_API SIA_IMPORT
#  endif
#endif

