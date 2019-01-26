#pragma once

#if defined(__APPLE__) && (defined(__GNUC__) || defined(__xlC__) || defined(__xlc__))
#  include <TargetConditionals.h>
#  if defined(TARGET_OS_MAC) && TARGET_OS_MAC
#    include <Availability.h>
#    include <AvailabilityMacros.h>
#    if defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
#      if 1
#        define SIA_OS_IOS
#      endif
#    else
#      define SIA_OS_OSX
#    endif
#  endif
#elif defined(__ANDROID__) || defined(ANDROID)
#  define SIA_OS_ANDROID
#elif (defined(WIN64) || defined(_WIN64) || defined(__WIN64__))
#  define SIA_OS_WIN32
#  define SIA_OS_WIN64
#elif (defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__))
#  define SIA_OS_WIN32
#elif defined(__linux__) || defined(__linux)
#  define SIA_OS_LINUX
#else
#  error "platform NOT supported!"
#endif

#if defined(SIA_OS_WIN32) || defined(SIA_OS_WIN64) || defined(SIA_OS_WINRT)
#  define SIA_OS_WIN
#endif

// linux
#ifdef __LSB_VERSION__
#  ifndef SIA_OS_LINUXBASE
#    define SIA_OS_LINUXBASE
#  endif
#endif

#ifdef SIA_OS_WIN
#  define SIA_EXPORT     __declspec(dllexport)
#  define SIA_IMPORT     __declspec(dllimport)
// c++ stl 模板未导出的问题
#pragma warning(disable:4251 4275)
#else
#  define SIA_EXPORT     __attribute__((visibility("default")))
#  define SIA_IMPORT     __attribute__((visibility("default")))
#endif
