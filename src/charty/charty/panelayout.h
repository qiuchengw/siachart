#pragma once

#include "graphy/layout.h"

namespace sia {
namespace charty {

class Panel;
class PanelLayout : public sia::graphy::Layout {
public:
    void setPanel(Panel* p);

protected:
    virtual bool parse(const SkString& layout) override;
    virtual void doLayout() override;

private:
    Panel* panel_ = nullptr;
};

}
}
