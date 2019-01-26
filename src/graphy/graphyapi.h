#pragma once

#include "siapi.h"

#ifdef STATIC_LIB
#  define GRAPHY_API
#else
#  if defined(graphy_EXPORTS)
#    define GRAPHY_API SIA_EXPORT
#  else
#    define GRAPHY_API SIA_IMPORT
#  endif
#endif

