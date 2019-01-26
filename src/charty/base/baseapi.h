#pragma once

#include "siapi.h"

#ifdef STATIC_LIB
#  define BASE_API
#else
#  if defined(base_EXPORTS)
#    define BASE_API SIA_EXPORT
#  else
#    define BASE_API SIA_IMPORT
#  endif
#endif

#include "base.h"
