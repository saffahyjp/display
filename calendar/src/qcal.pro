#-------------------------------------------------
#
# Project created by QtCreator 2016-08-22T19:35:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qcal
TEMPLATE = app

TRANSLATIONS += tr_zh_CN.ts

SOURCES += main.cpp\
    mainwindow.cpp \
    datamanager.cpp \
    dateinfo.cpp \
    calendar.cpp \
    datecell.cpp \
    datecellmenu.cpp \
    eventinfo.cpp \
    addeventdialog.cpp \
    addeventdialogwrapper.cpp \
    fileinfo.cpp \
    filemenu.cpp \
    mousebindings.cpp \
    mousebindingsdialog.cpp

HEADERS  += mainwindow.h \
    datamanager.h \
    dateinfo.h \
    calendar.h \
    datecell.h \
    datecellmenu.h \
    eventinfo.h \
    addeventdialog.h \
    addeventdialogwrapper.h \
    fileinfo.h \
    filemenu.h \
    mousebindings.h \
    mousebindingsdialog.h