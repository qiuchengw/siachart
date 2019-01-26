#pragma once

#include "graphy/layout.h"

namespace sia {
namespace charty {
class Panel;

class ChartLayout : public sia::graphy::Layout {
public:
    void setPanels(std::vector<Panel*>* panels);

protected:
    virtual bool parse(const SkString& layout) override;
    virtual void doLayout() override;

private:
    std::vector<Panel*>* panels_ = nullptr;
};

}
}