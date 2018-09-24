#-------------------------------------------------
#
# Project created by QtCreator 2017-01-26T20:05:22
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets webenginewidgets

TARGET = xmemo
TEMPLATE = app


SOURCES += main.cpp\
        XMemo.cpp \
    Settings.cpp \
    DbOperator.cpp \
    MemoInfo.cpp \
    MemoWidget.cpp \
    ./contribute/xswitch/xswitch.cpp

HEADERS  += XMemo.h \
    Settings.h \
    DbOperator.h \
    MemoInfo.h \
    MemoWidget.h \
    ./contribute/xswitch/xswitch.h

RESOURCES += \
    res.qrc

TRANSLATIONS    += zh_CN.ts

LIBS += -lhoedown
