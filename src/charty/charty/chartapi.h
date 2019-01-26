#pragma once

#include "siapi.h"

#ifdef STATIC_LIB
#  define CHARTY_API
#else
#  if defined(charty_EXPORTS)
#    define CHARTY_API SIA_EXPORT
#  else
#    define CHARTY_API SIA_IMPORT
#  endif
#endif

