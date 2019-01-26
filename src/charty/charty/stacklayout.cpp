#include "stacklayout.h"
#include "graphy/widget.h"

namespace sia {
namespace charty {

bool StackLayout::parse(const SkString& layout) {
    return true;
}

void dumpRect(const char* name, const SkRect& r);

void StackLayout::doLayout() {
    using namespace sia::graphy;

    SkRect rc = rect();
    for (graphy::Widget* w : childrenWidget()) {
        w->setGeometry(rc);
    }
}

}
}
