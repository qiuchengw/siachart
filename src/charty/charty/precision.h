#pragma once

#include "panelitem.h"

namespace sia {
namespace charty {

/*!
 * \class Precision
 *
 * \brief 用于计算图表的显示精度，除数因子等
 *
 */
class Precision {
public:
    Precision() {
        // 默认显示两位小数，无倍数。
        setPrecision(2, 1);
    }

    void setPrecision(int16_t precision, int32_t divisor) {
        precision_ = precision;
        divisor_ = divisor;
        precision_format_ = SkStringPrintf("%%.%dlf", precision);
    }

    inline int16_t precision()const {
        return precision_;
    }

    inline int32_t divisor()const {
        return divisor_;
    }

    /*! \brief 按照设定的精度格式化
     *
     *    @param	val	需要格式化的浮点数
     *    @return
     */
    inline SkString dataFormat(double val)const {
        if (precision_ <= 0) {
            return SkStringPrintf("%lld", (int64_t)val);
        }
        return SkStringPrintf(precision_format_.c_str(), val);
    }

    /*! \brief 数据除以除数因子后再格式化
     *
     *    @param	val	将会除以setPrecision中设置的divisor，然后调用dataFormat
     *    @return
     */
    inline SkString chartValFormat(chart_val_t val)const {
        if (!chartbase::validVal(val)) {
            return SkString("-");
        }
        return dataFormat(util::divide((double)val, (double)divisor_));
    }

private:
    // 显示精度
    int16_t precision_ = 2;
    SkString precision_format_;

    // 除数因子
    int32_t divisor_ = 1;
};

}
}
