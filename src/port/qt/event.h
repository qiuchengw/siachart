#pragma once

#include <QtCore>
#include <QtGui>
#include "../../graphy/event.h"

namespace sia {
namespace port {

inline qint32 transModifier(Qt::KeyboardModifiers mos) {
    static const QHash<Qt::KeyboardModifier, sia::ModifierKey> _modis = {
        { Qt::ShiftModifier, sia::kShift_ModifierKey },
        { Qt::ControlModifier, sia::kControl_ModifierKey },
        { Qt::AltModifier, sia::kOption_ModifierKey },
        { Qt::MetaModifier, sia::kCommand_ModifierKey },
    };

    qint32 ret = 0;
    for (auto i = _modis.begin(); i != _modis.end(); ++i) {
        if (mos & i.key()) {
            ret |= i.value();
        }
    }
    return ret;
}

inline sia::InputState transMouseInput(QEvent::Type typ) {
    switch (typ) {
    case QEvent::MouseMove:
        return sia::InputState::kMove_InputState;

    case QEvent::MouseButtonDblClick:
        return sia::InputState::kDoubleClick_InputState;

    case QEvent::MouseButtonPress:
        return sia::InputState::kDown_InputState;

    case QEvent::MouseButtonRelease:
        return sia::InputState::kUp_InputState;
    default:
        return sia::InputState::kMove_InputState;
    }
}

inline sia::KeyBoard transKey(int vk) {
    static const struct {
        qint32      fVK;
        sia::KeyBoard fKey;
    } gPair[] = {
        { Qt::Key_Back, sia::KeyBoard::kBack },
        { Qt::Key_Clear, sia::KeyBoard::kBack },
        { Qt::Key_Return, sia::KeyBoard::kOK },
        { Qt::Key_Up, sia::KeyBoard::kUp },
        { Qt::Key_Down, sia::KeyBoard::kDown },
        { Qt::Key_Left, sia::KeyBoard::kLeft },
        { Qt::Key_Right, sia::KeyBoard::kRight },
        { Qt::Key_Tab, sia::KeyBoard::kTab },
        { Qt::Key_PageUp, sia::KeyBoard::kPageUp },
        { Qt::Key_PageDown, sia::KeyBoard::kPageDown },
        { Qt::Key_Home, sia::KeyBoard::kHome },
        { Qt::Key_End, sia::KeyBoard::kEnd },
        { Qt::Key_Delete, sia::KeyBoard::kDelete },
        { Qt::Key_Escape, sia::KeyBoard::kEscape },
        { Qt::Key_Shift, sia::KeyBoard::kShift },
        { Qt::Key_Control, sia::KeyBoard::kCtrl },
        { Qt::Key_Menu, sia::KeyBoard::kOption },
        { 'A', sia::KeyBoard::kA },
        { 'C', sia::KeyBoard::kC },
        { 'V', sia::KeyBoard::kV },
        { 'X', sia::KeyBoard::kX },
        { 'Y', sia::KeyBoard::kY },
        { 'Z', sia::KeyBoard::kZ },
    };
    for (size_t i = 0; i < SK_ARRAY_COUNT(gPair); i++) {
        if (gPair[i].fVK == vk) {
            return gPair[i].fKey;
        }
    }
    return sia::KeyBoard::kNONE;
}

}   // port
}// sia
