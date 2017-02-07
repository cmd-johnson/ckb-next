TEMPLATE = app
TARGET = ckb-cloud

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.9
QMAKE_CXXFLAGS += -Werror

macx {
    DESTDIR = $$PWD/../../ckb.app/Contents/Resources/ckb-animations
} else {
    DESTDIR = $$PWD/../../bin/ckb-animations
}

CONFIG      += debug_and_release
QT           = core network gui

SOURCES += \
    main.cpp \
    bootstrapper.cpp \
    tcpclient.cpp \
    keyeffectmanager.cpp \
    keyeffects/keyeffect.cpp \
    keyeffects/fixedcolor.cpp \
    keyeffects/colorgradient.cpp \
    client.cpp \
    commandhandler.cpp

HEADERS += \
    bootstrapper.h \
    tcpclient.h \
    keyeffectmanager.h \
    keyeffects/keyeffect.h \
    keyeffects/fixedcolor.h \
    keyeffects/colorgradient.h \
    client.h \
    commandhandler.h
