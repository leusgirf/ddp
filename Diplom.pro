#-------------------------------------------------
#
# Project created by QtCreator 2014-04-19T03:48:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Diplom
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    parserS.cpp \
    sscetch.cpp \
    mscetcher.cpp \
    truescetch.cpp \
    beatscetch.cpp \
    operation.cpp \
    viewer.cpp \
    objects.cpp \
    planeobjects.cpp \
    tools.cpp \
    beats.cpp \
    calc.cpp \
    blanks.cpp \
    nplane.cpp \
    npage.cpp

HEADERS  += mainwindow.h \
    parserS.h \
    sscetch.h \
    mscetcher.h \
    truescetch.h \
    beatscetch.h \
    operation.h \
    viewer.h \
    objects.h \
    planeobjects.h \
    tools.h \
    beats.h \
    calc.h \
    blanks.h \
    nplane.h \
    npage.h

FORMS    += mainwindow.ui

RESOURCES += \
    resourses.qrc
CONFIG += c++11
