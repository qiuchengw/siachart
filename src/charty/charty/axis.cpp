#include "axis.h"

namespace sia {

namespace charty {

Axis::Axis(const SkString& name, Panel* parent)
    :PanelItem(name, parent) {
    setZIndex(VisualItemZValue::kAxis);
}

}

}

