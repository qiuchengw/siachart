#pragma once

#include "panelitem.h"
#include "precision.h"

namespace sia {
namespace charty {

class ValueSlice {
public:
    struct PosVal {
        PosVal(SkScalar y = 0.f, double v = 0.f)
            :ypos(y), val(v) {

        }

        // y 坐标
        SkScalar ypos;

        // 坐标处对应的值
        double val;

        //! val的format后的值
        SkString sval;

        //! 是否是中间值
        bool middle = false;
    };

    void setSlice(int count);

    /*! \brief 计算在垂直坐标上 [0，height]上，给出数值的平均等分位置
     *      （从上到下坐标值增大，而对应的数值减小）
     *
     *    @param    prec	数值转换以及format
     *    @param    height	能够使用的垂直空间
     *    @return
     */
    void calc(const Precision& prec, SkScalar height);

    const SkTArray<PosVal>& valPos()const {
        return valpos_;
    }

    const SkTArray<PosVal>& ratioPos()const {
        return ratiopos_;
    }

    inline chart_val_t middleVal()const {
        return middle_val_;
    }

    void setMiddleVal(chart_val_t val);

    void setMaxMin(chart_val_t min_y, chart_val_t max_y);

    inline chart_val_t minY()const {
        return min_y_;
    }

    inline chart_val_t maxY()const {
        return max_y_;
    }

    double valueOfYPos(SkScalar y, SkScalar height) const;

    SkScalar yPosOfValue(chart_val_t v, SkScalar height) const;

protected:
    void adjustSliceCount();

private:
    // 计算参数
    int16_t slice_ = 4;

    //! 中间值，用于标记
    chart_val_t middle_val_ = chartbase::kInvalidDataValue;

    // 最大最小值
    chart_val_t max_y_ = chartbase::kInvalidDataValue;
    chart_val_t min_y_ = chartbase::kInvalidDataValue;

private:
    // 计算结果缓存
    // value axis
    SkTArray<PosVal> valpos_;

    // ratio axis
    SkTArray<PosVal> ratiopos_;
};

}
}
