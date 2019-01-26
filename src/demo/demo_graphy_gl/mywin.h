#pragma once


#include "mode/gly/xapp.h"
#include "../mgraphy/widgets.h"

class MyWin : public sia::platy::XWindow {
public:
    MyWin();

protected:
    virtual void onBackendCreated() override {
        show(true);
    }

    virtual void onCreated() override;

private:
    SkScalar fRotationAngle = 0;
};

class MyApp : public sia::platy::XApp {
public:
    virtual void doCreate(int argc, char** argv, void* platformData) override {
        MyWin* w = new MyWin();
        if (w->create()) {
            setMainWindow(w);
        }
    }

    virtual void onIdle() override {
        mainWindow()->inval();
    }

};
