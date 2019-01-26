#ifndef locker_h__
#define locker_h__

#pragma once

#include <QMutex>
#include "macro.h"

// 本文件中的代码主要用于方便的实现数据的多线程安全保护
// 做到线程安全保护是有一定的使用限制的。
// 请注意结构/方法的使用注释
namespace threadsafe
{
    class QRecursiveLocker : public QMutex
    {
    public:
        QRecursiveLocker()
            :QMutex(QMutex::Recursive)
        {

        }
    };

    template<typename _C>
    class lock_ptr
    {
    public:
        typedef _C _Typ;

    public:
        explicit lock_ptr(QRecursiveLocker* m, _C* c)
            :m_(m), c_(c)
        {
            // static_assert(!std::is_pointer<_C>::value)
            if (nullptr != m_)
            {
                m_->lock();
            }
        }

        // 复制构造会转移控制权
        lock_ptr(const lock_ptr& t)
            :m_(t.m_), c_(t.c_)
        {
            const_cast<lock_ptr*>(&t)->m_ = nullptr;
            const_cast<lock_ptr*>(&t)->c_ = nullptr;
        }

        ~lock_ptr()
        {
            if (nullptr != m_)
            {
                m_->unlock();
                m_ = nullptr;
            }
        }

        _C * operator->()const
        {
            return c_;
        }

    private:
        QRecursiveLocker* m_ = nullptr;
        _C * c_ = nullptr;
    };

    // 要保证这个对象的创建是唯一的
    template <typename _C>
    class lock_obj
    {
    public:
        typedef _C _Typ;

    public:
        lock_obj()
        {
            static_assert(!std::is_pointer<_C>::value, "_C Must NOT be a pointer!");
        }

        lock_ptr<_C> lock()const
        {
            return lock_ptr<_C>(const_cast<QRecursiveLocker*>(&m_), const_cast<_C*>(&c_));
        }

        // 用这个函数省事儿，但是却保证不了线程安全
        lock_ptr<_C> operator->()const
        {
            return lock();
        }

    private:
        // 可递归重入锁，非递归锁会造成使用困难
        QRecursiveLocker m_;
        
        // 数据保存，不能是指针类型
        _C c_;
    };

// 
// 只有第三种使用方法是正确的：
//  先使用lock()获得临时对象，然后再去调用
//
// 1, 完全等同于第二种写法：begin/end 均产生一次锁和一次解锁。循环体内完全不受锁保护
//     for (const auto& t : obj)    
//     {
//         std::cout << t << "    ";
//     }
//
// 2, 完全同上
//     for (auto i = obj->begin(); i != obj->end(); ++i)
//     {
//         std::cout << *i << "    ";
//     }
//  
// 3, 因为有obj.lock() 产生的临时对象不会析构，所以不会解锁，循环体受到锁的保护
//     auto& v = obj.lock();
//     for (auto i = v->begin(); i != v->end(); ++i)
//     {
//         std::cout << *i << "    ";
//     }

    template <typename _C>
    class container_lock_obj : public lock_obj<_C>
    {
        typedef typename _C::iterator _Itr;
        typedef typename _C::const_iterator _ConsItr;

        typedef lock_obj<_C> _base;
    public:
        typedef _C _Typ;

    public:
        _Itr begin()
        {
            return _base::lock()->begin();
        }

        _Itr end()
        {
            return _base::lock()->end();
        }

        _ConsItr begin() const
        {
            return _base::lock()->cbegin();
        }

        _ConsItr end()const 
        {
            return _base::lock()->cend();
        }
    };

#define threadsafe_for(itr, cont) \
    static_assert(std::is_base_of<threadsafe::container_lock_obj<decltype(cont)::_Typ>, decltype(cont)>::value, \
            #cont##" ----------> !!! is NOT a container_lock_obj instance");\
    auto& _CAT_LINE(_v_) = cont.lock(); for(auto itr = _CAT_LINE(_v_)->begin(); itr != _CAT_LINE(_v_)->end(); ++itr)

#define threadsafe_for_each(val, cont) \
    static_assert(std::is_base_of<threadsafe::container_lock_obj<decltype(cont)::_Typ>, decltype(cont)>::value, \
            #cont##" ----------> !!! is NOT a container_lock_obj instance");\
    auto& _CAT_LINE(_v_) = cont.lock(); for(auto & val : cont)

}

#endif // locker_h__
