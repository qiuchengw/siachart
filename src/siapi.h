#pragma once

#ifdef _MSC_VER
#ifndef NOMINMAX
#define NOMINMAX
#endif
#endif

// #define ENABLE_VLD

#ifdef ENABLE_VLD
#include <vld.h>
#endif

#include <type_traits>
#include <functional>
#include <vector>
#include <unordered_map>
#include <memory>
#include <set>
#include <map>
#include <string>
#include <limits>
#include <cstdint>
#include <cassert>
#include <ctime>
#include <iostream>
#include <algorithm>

#include "SkString.h"
#include "SkPaint.h"
#include "SkImage.h"
#include "SkRect.h"
#include "SkCanvas.h"
#include "SkFontStyle.h"
#include "SkTypeface.h"
#include "SkPath.h"
#include "SkPathEffect.h"
#include "SkRegion.h"
#include "SkTextUtils.h"

#include "deps/os.h"
#include "deps/util.h"

#define SIA_API
#define DEBUG_CODE 1

#ifdef DEBUG_CODE
#include <iostream>
#endif

// private class
#define S_PRIV(C)

#ifdef _MSC_VER
// 低版本的MSVC
// #define HASH_WORK_AROUND(_Typ) \
//     namespace std{ template<> struct hash<_Typ> : public _Bitwise_hash<_Typ> {}; }

#define HASH_WORK_AROUND(_Typ) \
    namespace std{ template<> struct hash<_Typ> : public _Bitwise_hash<_Typ> {}; }
#else
#define HASH_WORK_AROUND(_Typ) \
    namespace std{\
    template<> struct hash<_Typ> : public __hash_base<size_t, _Typ> { \
    size_t operator()(const _Typ& v) const noexcept { return std::_Hash_impl::hash(int64_t(v)); }}; }
#endif

namespace std {
// 必须在使用之前包含！
// 比如unorder_map<SkString, ...>， 否则会有C2908错误
template<>
struct hash<SkString> {
    size_t operator()(const SkString& _Keyval) const {
        std::hash<std::string> proxy;
        return proxy(std::string(_Keyval.c_str()));
    }
};

template<>
struct less<SkString> {
    /*constexpr*/ bool operator()(const SkString& _Left, const SkString& _Right) const {
        // apply operator< to operands
        return (hash<SkString>()(_Left) < hash<SkString>()(_Right));
    }
};

}

namespace sia {
enum UIEvent {

    // widget event
    kEventWidgetDestroy = 0xFF00,
    kEventWidgetAttached,
};

// shared pointers
// skia 的sk_sp/SkRefCnt缺少了一个 shared_from_this 实现，用起来还是有很大限制
// 所以我们还是使用std::shared_ptr来取代它

#define my_sp std::shared_ptr
}

#define Sk_InvalidColor SkColor(0)

