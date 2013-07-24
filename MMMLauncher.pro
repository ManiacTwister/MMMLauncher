#-------------------------------------------------
#
# Project created by QtCreator 2013-07-21T22:37:12
#
#-------------------------------------------------

QT       += core gui network
CONFIG += console c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MMMLauncher
TEMPLATE = app


VERSION = 3.0
DEFINES += APPLICATION_VERSION=\"\\\"$$VERSION\\\"\"

SOURCES += main.cpp\
        mainwindow.cpp \
    filedownloader.cpp \
    aboutdialog.cpp \
    mmmlutils.cpp \
    epiparser.cpp \
    episode.cpp \
    category.cpp \
    epidownloader.cpp

HEADERS  += mainwindow.h \
    filedownloader.h \
    aboutdialog.h \
    mmmlutils.h \
    epiparser.h \
    episode.h \
    category.h \
    epidownloader.h

FORMS    += mainwindow.ui

RESOURCES += \
    mmml.qrc
