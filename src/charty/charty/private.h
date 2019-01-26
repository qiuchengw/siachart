#pragma once

#include "chart.h"
#include "chartlayout.h"
#include "crosshair.h"

namespace sia {
namespace charty {

struct ChartPrivate {
private:
    struct ViewCache {
        sk_sp<SkImage> snap;
        sk_sp<SkSurface> surf;

        // 拍个照
        bool snapshot(SkCanvas* canvas);

        // 绘制
        bool draw(SkCanvas* canvas);

        inline bool hasSnap()const {
            return !!snap;
        }
    };

public:
    enum Flags : int32_t {
        //! star
        kMinFlagShift = Chart::FunctionType::kFunctionMaxShift,

        //! 数据脏了，要计算新的图形，并全面重绘
        kDataDirty = 1 << (kMinFlagShift + 1),

        //! 视图脏了，只需要重绘视图，不用牵涉计算
        kViewDirty = 1 << (kMinFlagShift + 2),

        //! 数据脏了，要计算新的图形，并全面重绘
        kDomainChanged = 1 << (kMinFlagShift + 3),
    };

    // 布局管理
    ChartLayout layout;

    // 每个图表的实例都有一个id
    // id的具体含义由业务层来确定
    std::string id;

    // 每个图表包含一个或多个panel
    // 图表按添加顺序排序
    std::vector<Panel*> panels;

    // 上下文
    ChartContext ctx;

    //! 标志
    BitFlags flags;

    //! 十字线
    CrossHair cross;

    //! 当前选中的图元
    ViewItem* selected = nullptr;

    //! 鼠标状态检测
    util::MouseStateMachine mouse_state;

    //! 视图缓存
    ViewCache vcache;
};

}
}
