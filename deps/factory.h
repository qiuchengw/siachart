#pragma once

/*!
 * \brief 通用的工厂模式（factory pattern）实现。用于在运行期动态创建C++对象。
 *
 *	参考： https://github.com/google/infact
 *	这是一个简化的实现，够用就好 :D
 *	针对以下的模式使用：
 *		class Base {}
 *		class D1 : public Base {}
 *		class D2 : public Base {}
 *		...
 *		根据指定名字动态生成对象
 *	----------------
 *		FFactory<Base> ff;
 *		ff.add<D1>("D1");
 *		ff.add<D2>("D2");
 *		...
 *	-----------------
 *		创建：
 *		Base* d1 = ff.create("D1");
 */

#include <algorithm>
#include "SkString.h"

template<typename _Typ>
class _TrivalC {
public:
    virtual _Typ* create() = 0;
    virtual void destroy(_Typ*) = 0;
    virtual const SkString& typeName()const = 0;
};

/*!
 * \class TypeFactory
 *
 * \brief 类型工厂
 *		_Trival_Class，提供create /destroy 虚接口！
 */
template<typename _Typ, typename _BaseTyp>
class TypeFactory : public _TrivalC<_BaseTyp> {
    static_assert(std::is_base_of<_BaseTyp, _Typ>::value, "_Typ MUST be derived from _BaseTyp");

public:
    TypeFactory(const SkString& typ_name)
        :name_(typ_name) {
    }

    virtual ~TypeFactory() {
        for (auto w : objs_) {
            delete w;
        }
    }

    virtual _BaseTyp* create() override {
        _Typ *w = new _Typ(typeName());
        objs_.push_back(w);
        return w;
    }

    virtual void destroy(_BaseTyp* p) override {
        auto itr_end = objs_.end();
        auto i = std::remove(objs_.begin(), itr_end, p);
        if (i != itr_end) {
            delete p;
            objs_.pop_back_n(int(itr_end - i));
        }
    }

    virtual const SkString& typeName()const {
        return name_;
    }

protected:
    //! 已实例化的对象
    SkTArray<_BaseTyp*> objs_;

    //! 能够创建的类型名
    SkString name_;
};

/*!
 * \class FFactory
 *
 * \brief 类型工厂的工厂类
 *
 */
template<typename _BaseTyp>
class FFactory {
public:
    ~FFactory() {
        clear();
    }

    void clear() {
        for (auto f : factorys_) {
            delete f;
        }
        factorys_.reset();
    }

    /*! \brief 已注册的类型工厂
     *
     *    @param	names [out]	工厂名
     *    @return
     */
    void factoryNames(SkTArray<SkString>& names)const {
        for (auto t : factorys_) {
            names.push_back(t->typeName());
        }
    }

    template<class _FactoryItem>
    TypeFactory<_FactoryItem, _BaseTyp>* add(const SkString& name) {
        auto factory = new TypeFactory<_FactoryItem, _BaseTyp>(name);
        factorys_.push_back(factory);
        return factory;
    }

    _BaseTyp* create(const SkString& name) {
        if (auto f = findFactory(name)) {
            return f->create();
        }
        return nullptr;
    }

    void destory(const SkString& name, _BaseTyp* obj) {
        if (auto f = findFactory(name)) {
            return f->destroy(obj);
        }
    }

protected:
    /*! \brief 查找注册的类型工厂
     *
     *    @param	name	工厂名
     *    @return	类型工厂实例
     */
    _TrivalC<_BaseTyp>* findFactory(const SkString& name) {
        for (auto t : factorys_) {
            if (t->typeName() == name) {
                return t;
            }
        }
        return nullptr;
    }

private:
    //! 已注册的类型工厂实例
    SkTArray<_TrivalC<_BaseTyp>*> factorys_;
};
