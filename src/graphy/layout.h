#pragma once

#include "graphyapi.h"

// string
// HASH_WORK_AROUND(SkString);

const SkScalar INVALID_SCALAR_VAL = std::numeric_limits<float>::max();
const SkScalar MAX_SCALAR = std::numeric_limits<float>::max();
const SkScalar ZERO_SCALAR = SkIntToScalar(0);

namespace sia {
namespace graphy {
class Widget;
class GRAPHY_API Layout {
    friend class Widget;
public:
    virtual ~Layout();

    inline Widget* decoratedWidget()const {
        return decorated_;
    }

    SkRect geometry()const;

    SkRect rect()const;

protected:
    virtual bool parse(const SkString& layout) {
        return true;
    }

    virtual void doLayout() = 0;

    /*! \brief 设置需要应用此布局的控件
     *		设置的同时，内部会自动调用一次doLayout。上层可以不用再此调用doLayout
     *    @param
     *    @return
     */
    void setDecoratedWidget(Widget* w);

    Widget* widget(const char* name)const {
        return widget(SkString(name));
    }

    Widget* widget(const SkString& name)const;

    std::vector<Widget*> childrenWidget()const;

protected:
    Widget* decorated_ = nullptr;
};

class GRAPHY_API Measure {
    enum class MeasureType {
        kInvalid = 0,
        kFixed = 1,
        kPercent,
        kAll,	// 占用剩余所有的空间
    };

public:
    bool parse(const SkString& s);
    void set(SkScalar v, MeasureType t);

    // 宽度可以立即确定
    bool isImdiateSure()const {
        return isFixed() || isPercent();
    }

    bool isPercent()const {
        return type_ == MeasureType::kPercent;
    }

    bool isFixed()const {
        return type_ == MeasureType::kFixed;
    }

    SkScalar calc(SkScalar all, SkScalar max_v, SkScalar min_v)const;

    inline MeasureType typ()const {
        return type_;
    }

    inline SkScalar val()const {
        return val_;
    }

private:
    SkScalar val_ = 0.f;
    MeasureType type_ = MeasureType::kInvalid;
};

}
}
