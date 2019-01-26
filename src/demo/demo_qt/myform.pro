#-------------------------------------------------
#
# Project created by QtCreator 2018-08-13T13:33:30
#
#-------------------------------------------------

QT       += core gui
CONFIG += c++14 sdk_no_version_check

#winextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = myform
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS \
    NDEBUG

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TOP_DIR = $$PWD/../../../
SKIA_DIR = $$PWD/../../../../skiall

message("--->skia dir:")
message($$SKIA_DIR)

!exists($$SKIA_DIR){
    error("skia dependency not exists")
}

INCLUDEPATH += . \
    ../../ \
    $$SKIA_DIR/include/core \
    $$SKIA_DIR/include/views \
    $$SKIA_DIR/include/config \
    $$SKIA_DIR/include/private \
    $$SKIA_DIR/include/gpu \
    $$SKIA_DIR/include/utils \
    $$SKIA_DIR/include/encode \
    $$SKIA_DIR \
    $$SKIA_DIR/src/core\
    $$TOP_DIR \
    $$TOP_DIR/deps/lua \
    $$TOP_DIR/deps/lua/src

SOURCES += \
    main.cpp \
    chartpage.cpp \
    mychart.cpp \
    chartctrl.cpp \
    qcolorsel.cpp \
    indicatorpref.cpp \
    tdxdata.cpp \
    $$TOP_DIR/src/port/qt/imgconv.cpp

HEADERS += \
    chartpage.h \
    mychart.h \
    chartctrl.h \
    qcolorsel.h \
    indicatorpref.h \
    tdxdata.h \
    $$TOP_DIR/src/port/qt/imgconv.h \
    $$TOP_DIR/src/port/qt/event.h

LIBS += -L$$TOP_DIR/lib/ -L$$TOP_DIR/lib/debug -L$$SKIA_DIR/out/Shared/ -lrastery -lgraphy -lskia -lresy -lcharty -lformula -lbase

FORMS += \
    chartpage.ui
