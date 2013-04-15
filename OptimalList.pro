#-------------------------------------------------
#
# Project created by QtCreator 2013-02-13T22:30:24
#
#-------------------------------------------------

include("src/optimallist/optimallist.pri")

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OptimalList
TEMPLATE = app

RC_FILE = Resources.rc
RESOURCES += Resources.qrc

win32 {
    LIBS += -lpsapi
}

CONFIG (debug, debug|release) {
    DESTDIR = debug/
    OBJECTS_DIR = debug/gen
    MOC_DIR = debug/gen
    RCC_DIR = debug/gen
} else {
    DESTDIR = release/
    OBJECTS_DIR = release/gen
    MOC_DIR = release/gen
    RCC_DIR = release/gen
}

SOURCES +=  src/main.cpp \
            src/main/mainwindow.cpp \
            src/widgets/copyabletable.cpp \
            src/widgets/tablenumericitem.cpp

HEADERS  += src/main/mainwindow.h \
            src/main/types.h \
            src/widgets/copyabletable.h \
            src/widgets/tablenumericitem.h

FORMS    += src/main/mainwindow.ui
