#pragma once

#include "valueslice.h"

namespace sia {
namespace charty {
/*!
 * \class GridLine
 *
 * \brief 面板背景的网格线
 *
 *
 */
class GridLine {
    enum Flags {
        //! 显示垂直线
        kShowVertical = 0x1,

        //! 显示水平线
        kShowHorizontal = 0x2,

        //! kVerticalLineEqualSlice, kVerticalLineIndexSlice, kVerticalLineKeySlice 几个标志是互斥的。
        //! 垂直线使用等分
        kVerticalLineEqualSlice = 0x4,

        //! 垂直线使用visual index
        kVerticalLineIndexSlice = 0x8,

        //! 垂直线使用key
        kVerticalLineKeySlice = 0x10,
    };

public:
    GridLine();

    void draw(SkCanvas* canvas, MainView* view, SkScalar width, SkScalar height, const SkTArray<ValueSlice::PosVal>& hlines);
    void setPaints(SkPaint* grid);

    void showGrid(bool horizontal, bool vertical);
    void setGridVerticalSlice(int line_count);
    void setGridVerticalSlice(const SkTArray<chart_key_t>& keys_slice, chart_key_t middle_key);
    void setGridVerticalSlice(const SkTArray<int>& visual_indexes, int middle_index);

    inline bool hasHorizontalLine() const {
        return flags_.test(Flags::kShowHorizontal);
    }

    SkColor gridColor()const;

private:
    void setGridVerticalFlag(Flags f);

private:
    BitFlags flags_;

    //! 在visual index上分割面板
    SkTArray<int> v_index_slice_;
    int middle_index_ = -1;

    //! 使用key分割面板
    SkTArray<chart_key_t> keys_slice_;
    chart_key_t middle_key_ = chartbase::kInvalidDataKey;

    //! 使用等分
    int16_t v_slice_count_;

    // paints
    SkPaint* grid_ = nullptr;
    SkPaint middle_grid_;
};
}
}
