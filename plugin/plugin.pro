# Shared library without any Qt functionality
TEMPLATE = lib
QT -= gui core

CONFIG += console c++14 warn_on plugin
CONFIG -= thread qt

VERSION = 0.1.0

INCLUDEPATH += ../xplane_sdk/sdk/CHeaders/XPLM
INCLUDEPATH += ../xplane_sdk/sdk/CHeaders/Wrappers
INCLUDEPATH += ../xplane_sdk/sdk/CHeaders/Widgets
INCLUDEPATH += ../ppl/src
INCLUDEPATH += ../ppl/include

DEFINES += XPLM200 XPLM210

win32 {
# This has not been tested.
    DEFINES += APL=0 IBM=1 LIN=0
    LIBS += -L"$$PWD/../xplane_sdk/sdk/Libraries/Win"
    LIBS += -lXPLM_64 -lXPWidgets_64
    # LIBS += wherever PPL ends up
    TARGET = win.xpl
}

linux {
    DEFINES += APL=0 IBM=0 LIN=1
    LIBS += -L$$OUT_PWD/../ppl/lib
    CONFIG(debug, debug|release) {
        LIBS += -lppld
    } else {
        LIBS += -lppl
    }
    LIBS += -lGL -lGLEW
    TARGET = lin.xpl
    INCLUDEPATH += /usr/include/c++/4.8.4
    QMAKE_CXXFLAGS += -Wall -Wextra -Wfloat-equal -pedantic -Wno-c++11-narrowing
    QMAKE_CXXFLAGS += -fvisibility=hidden -std=c++14
}

macx {
# This has not been tested.
    DEFINES += APL=1 IBM=0 LIN=0
    TARGET = mac.xpl
    QMAKE_LFLAGS += -flat_namespace -undefined suppress
    CONFIG += x86 ppc
    # LIBS += wherever PPL ends up
}

HEADERS += \

SOURCES += \
    plugin.cpp \

DISTFILES += \
