#pragma once

#include "baseapi.h"

namespace sia {
namespace chartbase {

using data_key_t = int64_t;

// #define USE_INT64_DATA_VAL

#ifdef USE_INT64_DATA_VAL
using data_val_t = int64_t;
#else
using data_val_t = double;
#endif

#ifdef DONT_USE_THIS
// 不要启用这个代码。此代码只是为了让编辑器着色更好看点。嘿嘿
enum ConstDefs : int64_t {
    //! 无效chart_val_t
    kInvalidDataValue = std::numeric_limits<data_val_t>::max(),
    //! 无效 chart_key_t
    kInvalidDataKey = std::numeric_limits<data_val_t>::max(),
    //! 无效坐标
    kInvalidCoord,
};
#endif

#define MAX_AS_INVALID(T, DEF) \
    static const T DEF = std::numeric_limits<T>::max()

//! 无效chart_val_t
MAX_AS_INVALID(data_val_t, kInvalidDataValue);

//! 无效 chart_key_t
MAX_AS_INVALID(data_key_t, kInvalidDataKey);

//! 无效坐标
MAX_AS_INVALID(SkScalar, kInvalidCoord);
#undef MAX_AS_INVALID

inline bool validCoord(SkScalar x) {
    return kInvalidCoord != x;
}

inline bool validCoord(const SkPoint& p) {
    return validCoord(p.x()) && validCoord(p.y());
}

inline bool validKey(data_key_t k) {
    return kInvalidDataKey != k;
}

inline bool validVal(data_val_t v) {
    return kInvalidDataValue != v;
}

/*!
 * \enum sia::chartbase::SeriesType
 *
 *   图表序列类型
 */
enum class BASE_API SeriesType {
    kBlank = 0,
    kLine = 1,
    kBar = 2,
    kUpdownBar = 3,
    kCandle = 4,
};

}
}