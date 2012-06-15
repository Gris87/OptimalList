#-------------------------------------------------
#
# Project created by QtCreator 2012-06-13T21:02:44
#
#-------------------------------------------------

QT       += core gui

TARGET = OptimalList
TEMPLATE = app

RC_FILE = Resources.rc
RESOURCES += Resources.qrc

LIBS += -L"libs" -lpsapi

SOURCES += src/main.cpp\
           src/main/mainwindow.cpp

HEADERS  += src/main/mainwindow.h \
            src/list/optimallist.h

FORMS    += src/main/mainwindow.ui
