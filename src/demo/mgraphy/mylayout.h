#pragma once

#include "graphy/layout.h"

namespace test {
class MyLayout : public sia::graphy::Layout {
public:

protected:
    virtual bool parse(const SkString& layout) override;
    virtual void doLayout() override;
};
}