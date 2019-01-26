#pragma once

#include "graphy/layout.h"

namespace sia {
namespace charty {

class StackLayout : public sia::graphy::Layout {
public:

protected:
    virtual bool parse(const SkString& layout) override;
    virtual void doLayout() override;
};

}
}