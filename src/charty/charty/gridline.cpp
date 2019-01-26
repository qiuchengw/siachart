#include "gridline.h"
#include "mainview.h"
#include "SkDashPathEffect.h"

namespace sia {

namespace charty {


GridLine::GridLine() {

}

void GridLine::draw(SkCanvas* canvas, MainView* view, SkScalar width, SkScalar height,
                    const SkTArray<ValueSlice::PosVal>& hlines) {
    // 画grid，横线
    if (flags_.test(Flags::kShowHorizontal)) {
        if (grid_->getPathEffect()) {
            SkPath path;
            for (auto &pv : hlines) {
                path.moveTo(0, pv.ypos);
                path.lineTo(width, pv.ypos);
            }
            canvas->drawPath(path, *grid_);
        } else {
            for (auto &pv : hlines) {
                canvas->drawLine(0, pv.ypos, width, pv.ypos, pv.middle ? middle_grid_ : *grid_);
            }
        }
    }
    return;

    // 画grid垂直线
    if (flags_.test(Flags::kShowVertical)) {
        if (flags_.test(Flags::kVerticalLineEqualSlice)) {
            const SkScalar V_SLICE_WIDTH = util::divide(width, SkIntToScalar(v_slice_count_));
            int middle_i = (v_slice_count_ % 2 == 0) ? (v_slice_count_ / 2) : -1; // 只有当等分的时候才有中线
            for (int i = 1; i < v_slice_count_; ++i) {	// 第0个和最后一个总是画到边框上，就不要再画了
                SkScalar x = i * V_SLICE_WIDTH;
                canvas->drawLine(x, 0, x, height, ((middle_i == i) ? middle_grid_ : *grid_));
            }
        } else if (flags_.test(Flags::kVerticalLineIndexSlice)) {
            for (int idx : v_index_slice_) {
                SkScalar x = view->xPosOfVisualIndex(idx);
                canvas->drawLine(x, 0, x, height, (middle_index_ == idx) ? middle_grid_ : *grid_);
            }
        } else if (flags_.test(Flags::kVerticalLineKeySlice)) {
            for (chart_key_t key : keys_slice_) {
                SkScalar x = view->xPosOfKey(key);
                canvas->drawLine(x, 0, x, height, (middle_key_ == key) ? middle_grid_ : *grid_);
            }
        }
    }
}

void GridLine::setPaints(SkPaint* grid) {
    if (!grid) {
        return;
    }

    grid_ = grid;
    grid_->setStyle(SkPaint::kStroke_Style);

    // 中间的线要宽一点
    middle_grid_ = *grid_;
    middle_grid_.setStrokeWidth(grid_->getStrokeWidth() * 2 + 1);
}

void GridLine::showGrid(bool horizontal, bool vertical) {
    flags_.set(Flags::kShowHorizontal, horizontal);
    flags_.set(Flags::kShowVertical, vertical);
}

void GridLine::setGridVerticalSlice(int line_count) {
    setGridVerticalFlag(Flags::kVerticalLineEqualSlice);
    v_slice_count_ = line_count;
}

void GridLine::setGridVerticalSlice(const SkTArray<int>& visual_indexes, int middle_index) {
    setGridVerticalFlag(Flags::kVerticalLineIndexSlice);
    v_index_slice_ = visual_indexes;
    middle_index_ = middle_index;
}

void GridLine::setGridVerticalSlice(const SkTArray<chart_key_t>& keys_slice, chart_key_t middle_key) {
    setGridVerticalFlag(Flags::kVerticalLineKeySlice);
    keys_slice_ = keys_slice;
    middle_key_ = middle_key;
}

SkColor GridLine::gridColor() const {
    if (grid_) {
        return grid_->getColor();
    }
    return SK_ColorRED;
}

void GridLine::setGridVerticalFlag(Flags f) {
    flags_.set(Flags::kVerticalLineKeySlice, false);
    flags_.set(Flags::kVerticalLineEqualSlice, false);
    flags_.set(Flags::kVerticalLineIndexSlice, false);

    flags_.set(f, true);
}

}

}

