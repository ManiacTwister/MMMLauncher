#-------------------------------------------------
#
# Project created by QtCreator 2013-07-21T22:37:12
#
#-------------------------------------------------

QT       += core gui network
CONFIG += qt console c++11
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mmmlauncher
TEMPLATE = app


VERSION = "3.0.6"
USERAGENT = MMMLauncher/$$VERSION
DEFINES += APPLICATION_VERSION=\"\\\"$$VERSION\\\"\"
DEFINES += USERAGENT=\"\\\"$$USERAGENT\\\"\"

win32:RC_FILE = MMMLauncher.rc

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
    Parsers/authorparser.cpp \
    version.cpp

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
    Parsers/authorparser.h \
    version.h

FORMS    += mainwindow.ui \
    newtabdialog.ui

RESOURCES += \
    mmml.qrc

binfile.files += mmmlauncher
binfile.path = /usr/bin/
shortcutfiles.files += data/MMMLauncher.desktop
shortcutfiles.path = /usr/share/applications/
pixmapfile.path = /usr/share/icons/hicolor/32x32/apps/
pixmapfile.files = mmmlauncher.png
INSTALLS += shortcutfiles
INSTALLS += binfile
INSTALLS += pixmapfile
