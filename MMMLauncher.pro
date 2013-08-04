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
    epidownloader.cpp \
    qextract.cpp \
    newtabdialog.cpp \
    Models/author.cpp \
    Models/episode.cpp \
    Models/category.cpp \
    Parsers/categoryparser.cpp \
    Parsers/epiparser.cpp \
    Parsers/authorparser.cpp

HEADERS  += mainwindow.h \
    filedownloader.h \
    aboutdialog.h \
    mmmlutils.h \
    epidownloader.h \
    qextract.h \
    newtabdialog.h \
    Models/author.h \
    Models/episode.h \
    Models/category.h \
    Parsers/categoryparser.h \
    Parsers/epiparser.h \
    Parsers/authorparser.h

FORMS    += mainwindow.ui \
    newtabdialog.ui

RESOURCES += \
    mmml.qrc
