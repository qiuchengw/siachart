#pragma once

#include "bitflag.h"
#include "graphy/event.h"

namespace util {

/*!
 * \class MouseStateMachine
 *
 * \brief 检测鼠标状态，简化程序中分散的逻辑判断
 *		暂不支持鼠标的左右键等区分判断
 */
class MouseStateMachine {
    enum Flags {
        //! 鼠标按下
        kButtonPressed = 1 << 1,

        //! 拖拽中：鼠标按下且移动
        kDragging = 1 << 2,

        //! 拖拽结束：拖拽中，鼠标按下状态弹起
        kDragEnd = 1 << 3,
    };

public:
    void onMouse(int x, int y, sia::InputState state, uint32_t modifiers) {
        switch (state) {
        case sia::InputState::kDoubleClick_InputState:
        case sia::InputState::kClick_InputState:
            flags.clear();
            break;

        case sia::InputState::kDown_InputState:
            flags.set(Flags::kButtonPressed, true);
            break;

        case sia::InputState::kMove_InputState:
            if (flags.test(Flags::kButtonPressed)) {
                flags.set(Flags::kDragging, true);
            }
            break;

        case sia::InputState::kUp_InputState:
            if (dragging()) {
                flags.set(Flags::kDragEnd, true);
            }
            // 不再拖动和按下状态了
            flags.set(Flags::kButtonPressed | Flags::kDragging, false);
            break;
        }
    }

    inline bool dragged()const {
        return flags.test(Flags::kDragEnd);
    }

    inline bool dragging()const {
        return flags.test(Flags::kDragging);
    }

    inline bool pressing()const {
        return flags.test(Flags::kButtonPressed);
    }

private:
    BitFlags flags;
};

}

