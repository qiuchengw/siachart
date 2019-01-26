#pragma once

#include "formulapi.h"
#include "../base/base.h"

namespace sia {
namespace formula {

/*!
 * \class TaVariable
 *
 * \brief 公式输入参数的值
 *
 */
struct FORMULA_API TaVariable {
    SkString name;
    SkTArray</*TA_Real*/double> vd;

    template<typename T>
    T asT()const {
        if (vd.empty()) {
            return T();
        }
        return (T)(vd.front());
    }

    template<typename T>
    std::vector<T> asList()const {
        std::vector<T> ret;
        for (auto t : vd) {
            ret.push_back(reinterpret_cast<T>(t));
        }
        return ret;
    }

    void set(double val);

    void push(double val);
};

/*!
 * \class TaSeries
 *
 * \brief 公式输出序列
 *
 */
struct FORMULA_API TaSeries {
    // 序列名
    SkString series_name;

    //! 序列需要的数据名，公式输出的？或者原有的数据均可（只要存在）
    SkTArray<SkString> data_name;

    //! 序列类型
    chartbase::SeriesType typ;

    //! 序列的颜色定制
    SkColor color = SK_ColorCYAN;
};

/*!
 * \class TaParam
 *
 * \brief 公式输入/输出参数
 *
 */
class FORMULA_API TaParam {
public:
    TaParam() {
    }

    template<typename T>
    T asT(const SkString& name)const {
        for (const TaVariable& t : in_) {
            if (t.name.equals(name)) {
                return t.asT<T>();
            }
        }
        return T();
    }

    template<typename T>
    std::vector<T> asList(const SkString& name)const {
        for (const TaVariable& t : in_) {
            if (t.name.equals(name)) {
                return t.asList<T>();
            }
        }
        return std::vector<T>();
    }

    void setParam(const SkString& name, double val);

    void pushParam(const SkString& name, double val);

    void addOutSeries(const TaSeries& series);

    const SkTArray<TaSeries>& outSeries()const;

    /*! \brief 参数列表简写形式。
     *
     *    @param	with_name	true:带上参数名字
     *    @return	形如：(1,2,3) / with_name: (signal:12, fast:26)
     */
    SkString paramBrief(bool with_name)const;

protected:
    void insert(const SkString& name, double val);

private:
    //！公式需要的输入参数。可能有多个
    SkTArray<TaVariable> in_;

    // 公式处理后的图表显示（此参数可选）
    // 1. 一个公式可能有多个类型的序列输出，比如MACD有两种类型（1*updown + 2*line）序列输出
    // 2. 公式运算完成后也可以完全不输出数据（更不会产生图表序列）
    SkTArray<TaSeries> out_series_;
};

}
}
