#-------------------------------------------------
#
# Project created by QtCreator 2016-08-30T09:56:41
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = five
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    socketstream.cpp \
    ssgetter.cpp \
    game.cpp \
    board.cpp \
    data.cpp \
    serverdialog.cpp \
    clientdialog.cpp

HEADERS  += mainwindow.h \
    socketstream.h \
    ssgetter.h \
    game.h \
    board.h \
    data.h \
    serverdialog.h \
    clientdialog.h
