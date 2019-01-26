#include "xapp.h"
#include "SkGraphics.h"
#include "graphy/paintman.h"

namespace sia {
namespace platy {

XApp* _s_xapp = nullptr;

XApp::XApp() {
    _s_xapp = this;

    paintman_ = std::make_shared<graphy::PaintMan>();
    paintman_->init();

    SkGraphics::Init();
}

XApp::~XApp() {
}

void XApp::setMainWindow(XWindow* w) {
    mainwin_ = w;

    // ¼ÓÔØ»æÖÆÅäÖÃ
    loadPaintConfig(SkString("./paint.json"));
}

void XApp::onIdle() {

}

void XApp::loadPaintConfig(const SkString& cfgpath) {
    paintman_->load(cfgpath);
}

}
}