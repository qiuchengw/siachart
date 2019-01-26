#ifndef MACRO_H__
#define MACRO_H__

#pragma once

#include <functional>

// helper macro
//  gcc 实现的std库中，有很多参数名直接就是 __in / __out 
//  和windows定义的 __in 宏重合了，所以我们自己定义一个宏
#ifndef __p_in
#define __p_in
#define __p_out
#define __p_inout
#endif

#ifdef  Q_OS_UNIX 
#   ifdef __x86_64__
#       ifndef __LP64__
#       define __LP64__
#       endif
#   else
#       ifndef __LP32__
#       define __LP32__
#       endif
#   endif
#endif

#ifdef QT_CORE_LIB

#include <QtCore/qglobal.h>

// VERIFY 宏
#if !defined(Q_VERIFY)
# if !defined(QT_NO_DEBUG)
#  define Q_VERIFY Q_ASSERT
# else
#  define Q_VERIFY(expr)  \
    do                    \
            {                     \
        (void) (expr);    \
            } while (0)
# endif
#endif

#endif // QT_DLL

// QString' data
#define String2PStr(s) (s.toUtf8().constData())

//////////////////////////////////////////////////////////////////////////
// 转换相对路径到绝对路径
//      转换路径基于使用此宏的文件
// eg:
//  E:\dir\a.h 中使用此宏
//      _CONVERT_2_ABSPATH(lib/test.lib)
//  宏展开后为：
//      E:\dir\a.h/../lib/test.lib  == E:/dir/lib/test.lib
#ifndef _CONVERT_2_ABSPATH
    #define _CONVERT_2_ABSPATH(f) __FILE__##"/../"#f
#endif

// 链接到lib文件（lib文件路径为使用此宏的文件的相对路径）
//  eg:
//      #pragma _LINK_LIBFILE(lib/test.lib)
#ifndef _LINK_LIBFILE
    #define _LINK_LIBFILE(f) comment(lib, _CONVERT_2_ABSPATH(f))
#endif 

// 连接字面
#define _CAT(a,b) a##b

// 连接变量
#define _CAT_VAR(a,b) _CAT(a, b)

// 连接名字和行号
#define _CAT_LINE(a) _CAT_VAR(a, __LINE__)

// ToString
#define E2S(x) #x 


// 释放管理器中的所有元素
#define DELETE_ALL_ELEMENT(container_) \
    for each(auto *__ in container_) { delete __; }

#define SAFE_DELETE(p) do {delete p; p = nullptr; }while(0)
#define DELETE_CONTAINER_ELEM(c) do { for (auto* p : c){delete p;} c.clear(); }while(0)

//////////////////////////////////////////////////////////////////////////
// DATE time formate
#define DATETIME_FORMAT_S "yyyy-MM-dd HH:mm:ss"
#define DATETIME_FORMAT_M "yyyy-MM-dd HH:mm"
#define DATE_FORMAT "yyyy-MM-dd"
#define TIME_FORMAT "HH:mm:ss"

/**
*  资源管理： 申请和释放保护
*
*  实现摘录自：C++罗浮宫
*
*      http://mindhacks.cn/2012/08/27/modern-cpp-practices/
*
**/
class ScopeGuard
{
public:
    explicit ScopeGuard(std::function<void()> onExitScope)
        : onExitScope_(onExitScope), dismissed_(false)
    { }

    ~ScopeGuard()
    {
        if (!dismissed_)
        {
            onExitScope_();
        }
    }

    void Dismiss()
    {
        dismissed_ = true;
    }

private:
    std::function<void()> onExitScope_;
    bool dismissed_;

private: // noncopyable
    ScopeGuard(ScopeGuard const&);
    ScopeGuard& operator=(ScopeGuard const&);
};

// HANDLE h = CreateFile(...);
// ScopeGuard onExit([&] { CloseHandle(h); });

#define SCOPEGUARD_LINENAME_CAT(name,line) name##line
#define SCOPEGUARD_LINENAME(name, line) SCOPEGUARD_LINENAME_CAT(name, line)

#define ON_SCOPE_EXIT(callback) ScopeGuard SCOPEGUARD_LINENAME(EXIT, __LINE__)(callback)

#endif // MACRO_H__
