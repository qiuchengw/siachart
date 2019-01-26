#include "panelitem.h"
#include "panel.h"
#include "mainview.h"
#include "series.h"

namespace sia {

namespace charty {


PanelItem::PanelItem(const SkString& name, Panel* parent)
    :VisualItem(name, (Widget*)parent) {

}

Panel* PanelItem::parent() const {
    return reinterpret_cast<Panel*>(VisualItem::parent());
}

MainView* PanelItem::mainView() const {
    return panel()->mainView();
}

std::vector<my_sp<Series>> PanelItem::serieses() const {
    return panel()->serieses();
}

}

}

