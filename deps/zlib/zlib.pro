#-------------------------------------------------
#
# Project created by QtCreator 2017-05-12T16:27:02
#
#-------------------------------------------------

QT       -= core gui

TARGET = zlib
TEMPLATE = lib

DEFINES += ZLIB_DLL ZLIB_INTERNAL

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(../../common.pri)

HEADERS += \
    crc32.h \
    deflate.h \
    inffast.h \
    inffixed.h \
    inflate.h \
    inftrees.h \
    resource.h \
    trees.h \
    zconf.h \
    zconf.in.h \
    zlib.h \
    zutil.h

SOURCES += \
    adler32.c \
    compress.c \
    crc32.c \
    deflate.c \
    gzio.c \
    infback.c \
    inffast.c \
    inflate.c \
    inftrees.c \
    minigzip.c \
    trees.c \
    uncompr.c \
    zutil.c

