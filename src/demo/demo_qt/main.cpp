#include <QApplication>
#include "graphy/paintman.h"
#include "chartpage.h"
#include <QDir>
#include <QDebug>

my_sp<sia::graphy::PaintMan> init_paintman() {
    my_sp<sia::graphy::PaintMan> man = std::make_shared<sia::graphy::PaintMan>();
    man->init();

    qDebug() << QDir::currentPath();

    if (!man->load(SkString("./black_theme.lua"))) {
        return nullptr;
    }
    man->dumpInfo();
    return man;
}

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    if (!init_paintman()) {
        return -1;
    }

    ChartPage w;
    w.show();

    return a.exec();
}
