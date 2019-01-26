#pragma once

#include "siapi.h"


#ifdef STATIC_LIB
#  define FORMULA_API
#else
#  if defined(formula_EXPORTS)
#    define FORMULA_API SIA_EXPORT
#  else
#    define FORMULA_API SIA_IMPORT
#  endif
#endif

