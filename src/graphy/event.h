#pragma once

#include "graphyapi.h"

// Scene 用于管理基本的组件试图
// 并和系统Surface/Window做交互
namespace sia {

// input handling
enum class GRAPHY_API KeyBoard {
    kNONE,    //corresponds to android's UNKNOWN

    kLeftSoftKey,
    kRightSoftKey,

    kHome,    //!< the home key - added to match android
    kBack,    //!< (CLR)
    kSend,    //!< the green (talk) key
    kEnd,     //!< the red key

    k0,
    k1,
    k2,
    k3,
    k4,
    k5,
    k6,
    k7,
    k8,
    k9,
    kStar,    //!< the * key
    kHash,    //!< the # key

    kUp,
    kDown,
    kLeft,
    kRight,

    // Keys needed by ImGui
    kTab,
    kPageUp,
    kPageDown,
    kDelete,
    kEscape,
    kShift,
    kCtrl,
    kOption, // AKA Alt
    kA,
    kC,
    kV,
    kX,
    kY,
    kZ,

    kOK,      //!< the center key

    kVolUp,   //!< volume up    - match android
    kVolDown, //!< volume down  - same
    kPower,   //!< power button - same
    kCamera,  //!< camera       - same

    kLast = kCamera
};
static const int kKeyCount = static_cast<int>(KeyBoard::kLast) + 1;

enum /*class*/ GRAPHY_API ModifierKey {
    kShift_ModifierKey = 1 << 0,
    kControl_ModifierKey = 1 << 1,
    kOption_ModifierKey = 1 << 2,   // same as ALT
    kCommand_ModifierKey = 1 << 3,

    // The previous key state.
    // The value is 1 if the key is down before the message is sent, or it is 0 if the key is up.
    kFirstPress_ModifierKey = 1 << 4,
};

enum class GRAPHY_API InputState {
    kDown_InputState,
    kUp_InputState,
    kClick_InputState,  // only valid for mouse
    kDoubleClick_InputState,    // only valid for mouse
    kMove_InputState   // only valid for mouse
};

}
