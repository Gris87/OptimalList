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

LIBS += -lpsapi

SOURCES +=  src/main.cpp\
            src/main/mainwindow.cpp \
            src/list/optimallist.cpp \
            src/widgets/copyabletable.cpp \
            src/widgets/tablenumericitem.cpp

HEADERS  += src/main/mainwindow.h \
            src/main/types.h \
            src/list/optimallist.h \
            src/widgets/copyabletable.h \
            src/widgets/tablenumericitem.h

FORMS    += src/main/mainwindow.ui
