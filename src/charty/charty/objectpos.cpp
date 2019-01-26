#include "objectpos.h"
#include "mainview.h"
#include "viewitem.h"

namespace sia {
namespace charty {

bool PositionCalc::calc(MainView* view, SkSize sz, SkRect& object_rect) {
    if (!view || PosType::kNotSet == pos_) {
        return false;
    }
    object_rect = SkRect::MakeEmpty();
    sz_ = sz;
    view_ = view;

    switch (pos_) {
    case PosType::kRelated:
        object_rect = calcRelate();
        break;

    case PosType::kValued:
        object_rect = calcDataVal();
        break;

    case PosType::kAttached:
        object_rect = calcAttach();
        break;
    }

    if (object_rect.isEmpty() || !view->rect().contains(object_rect)) {
        return false;
    }
    return true;
}

void PositionCalc::setAttached(ViewItem* item, chart_key_t x) {
    pos_ = PosType::kAttached;
    attach_ = item;
    assert(attach_);
    x_ = x;
}

SkRect PositionCalc::calcRelate() {
    const SkScalar MAX_W = view_->contentWidth();
    const SkScalar MAX_H = view_->contentHeight();

    const SkScalar ITEM_W = sz_.width();
    const SkScalar ITEM_H = sz_.height();

#define COOR_L(x) (x)
#define COOR_R(x) (MAX_W - x)
#define COOR_T(x) (x)
#define COOR_B(x) (MAX_H - x)

    // 最终的值
    RelPos r = rel_;
    if (!validCoord(r.l)) {
        // 左侧位置不知道
        if (!validCoord(r.r)) {
            // 右侧位置也不知道，那么我们就用size
            r.l = (MAX_W - ITEM_W) / 2;
            r.r = r.l;  // 两边一样的距离
        } else {
            // 知道右侧的位置，根据宽度推测左侧的
            r.l = COOR_R(r.r) - ITEM_W;
        }
    } else {
        if (!validCoord(r.r)) {
            r.r = MAX_W - ITEM_W - COOR_L(r.l);
        }
    }

    if (!validCoord(r.t)) {
        // 不知道上方的位置
        if (!validCoord(r.b)) {
            // 也不知道下方的，根据高度推测上方的
            r.t = (MAX_H - ITEM_H) / 2;
            r.b = r.t;
        } else {
            // 知道下方的，根据高度推测上方
            r.t = COOR_B(r.b) - ITEM_H;
        }
    } else {
        if (!validCoord(r.b)) {
            r.b = MAX_H - ITEM_H - COOR_L(r.t);
        }
    }

#undef COOR_L
#undef COOR_T
#undef COOR_R
#undef COOR_B

    // 到这个地方的时候，ltrb应该都是已经知道的了
    return SkRect::MakeLTRB(r.l, r.t, MAX_W - r.r, MAX_H - r.b);
}

extern SkRect adjustRect(const SkPoint& pt, const SkSize& target, const SkRect& cont, SkPoint& pt_start);

SkRect PositionCalc::calcAttach() {
    // 先计算出来 attach_ 的位置
    if (!view_ || !attach_ || !attach_->rectOfKey(x_, item_rc_)) {
        return SkRect::MakeEmpty();
    }
    item_point_.set(item_rc_.centerX(), item_rc_.centerY());
    return adjustRect(item_point_, sz_, view_->contentRect(), object_point_);

//     enum /*class*/ _MyFlags {
//         kTopLeft = 0x1,
//         kTopRight = 0x2,
//         kBottomLeft = 0x4,
//         kBottomRight = 0x8,
//     };
//
//     // 我们要进行标注，标注区不能落到可视区的外面。所以要调整下矩形大小
//     // 首先看下，上下左右，哪边留的空隙最大
//     const SkScalar MAX_W = view_->contentWidth();
//     const SkScalar MAX_H = view_->contentHeight();
//     const SkScalar ITEM_W = sz_.width();
//     const SkScalar ITEM_H = sz_.height();
//
//     BitFlags flags;
//     SkPoint obj_top_left;
//     const SkScalar SPACING = 10;
//     if (item_rc_.left() > (MAX_W - item_rc_.right())) {
//         // 在左边
//         if (item_rc_.top() >= (MAX_H - item_rc_.bottom())) {
//             // 左下
//             obj_top_left.set(item_rc_.left() - ITEM_W - SPACING, item_rc_.bottom() + SPACING);
//             item_point_.set(item_rc_.left(), item_rc_.bottom());
//             flags.set(_MyFlags::kBottomLeft);
//         } else {
//             // 左上
//             obj_top_left.set(item_rc_.left() - ITEM_W - SPACING, item_rc_.top() - ITEM_H - SPACING);
//             item_point_.set(item_rc_.left(), item_rc_.top());
//             flags.set(_MyFlags::kTopLeft);
//         }
//     } else { // 右边
//         if (item_rc_.top() >= (MAX_H - item_rc_.bottom())) {
//             // 右上
//             obj_top_left.set(item_rc_.right() + SPACING, item_rc_.top() - ITEM_H - SPACING);
//             item_point_.set(item_rc_.right(), item_rc_.top());
//             flags.set(_MyFlags::kTopRight);
//         } else {
//             // 右下
//             obj_top_left.set(item_rc_.right() + SPACING, item_rc_.bottom() + SPACING);
//             item_point_.set(item_rc_.right(), item_rc_.bottom());
//             flags.set(_MyFlags::kBottomRight);
//         }
//     }
//     SkRect rc = SkRect::MakeXYWH(obj_top_left.x(), obj_top_left.y(), ITEM_W, ITEM_H);
//
//     // 看看是否超过了可视区边界
//     if (rc.left() < 0) {
//         rc.offsetTo(0, rc.top());
//     }
//     if (rc.top() < 0) {
//         rc.offsetTo(rc.left(), 0);
//     }
//     if (rc.right() > MAX_W) {
//         rc = rc.makeOffset(rc.right() - MAX_W, 0);
//     }
//     if (rc.bottom() > MAX_H) {
//         rc = rc.makeOffset(0, rc.bottom() - MAX_H);
//     }
//
//     // 找出合适的连接线点
//     // 连接点肯定是对立的！比如，左下要和右上连接，右下要和左上链接
//     if (flags.test(_MyFlags::kTopLeft)) {
//         object_point_.set(rc.right(), rc.bottom());
//     }
//     if (flags.test(_MyFlags::kTopRight)) {
//         object_point_.set(rc.left(), rc.bottom());
//     }
//     if (flags.test(_MyFlags::kBottomRight)) {
//         object_point_.set(rc.left(), rc.top());
//     }
//     if (flags.test(_MyFlags::kBottomLeft)) {
//         object_point_.set(rc.right(), rc.top());
//     }
//     return rc;
}

SkRect PositionCalc::calcDataVal() {
    return SkRect::MakeEmpty();
}

bool PositionCalc::validCoord(SkScalar coo) const {
    return !util::isEqual(coo, chartbase::kInvalidCoord);
}

}
}

