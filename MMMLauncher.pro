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

OS_T = "Unknown"
win32:OS_T = "Windows"
unix:OS_T = "Unix"

VERSION = "3.1.4"
USERAGENT = MMMLauncher/$$VERSION/$$OS_T
DEFINES += APPLICATION_VERSION=\"\\\"$$VERSION\\\"\"
DEFINES += USERAGENT=\"\\\"$$USERAGENT\\\"\"

defineReplace(prependAll) {
 for(a,$$1):result += $$2$${a}$$3
 return($$result)
}
LANGUAGES = de en
TRANSLATIONS = $$prependAll(LANGUAGES, $$PWD/languages/mmml_, .ts)
TRANSLATIONS_FILES =

qtPrepareTool(LRELEASE, lrelease)
for(tsfile, TRANSLATIONS) {
 qmfile = $$shadowed($$tsfile)
 qmfile ~= s,\\.ts$,.qm,
 qmdir = $$dirname(qmfile)
 !exists($$qmdir) {
 mkpath($$qmdir)|error("Aborting.")
 }
 command = $$LRELEASE -removeidentical $$tsfile -qm $$qmfile
 system($$command)|error("Failed to run: $$command")
 TRANSLATIONS_FILES+= $$qmfile
 lang = $$replace(tsfile, .*_([^/]*)\\.ts, \\1)
}

isEmpty(QMAKE_LRELEASE) {
    win32:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\lrelease.exe
    else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease
}
updateqm.input = TRANSLATIONS
updateqm.output = ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}.qm
updateqm.commands = $$QMAKE_LRELEASE ${QMAKE_FILE_IN} -qm ${QMAKE_FILE_PATH}/${QMAKE_FILE_BASE}.qm
updateqm.CONFIG += no_link
QMAKE_EXTRA_COMPILERS += updateqm
PRE_TARGETDEPS += compiler_updateqm_make_all


GENERATED_RESOURCE_FILE = $$OUT_PWD/translations.qrc
RESOURCE_CONTENT = \
    "<RCC>" \
    "<qresource>"
for(translationfile, TRANSLATIONS_FILES) {
    relativepath_out = $$relative_path($$translationfile, $$OUT_PWD)
    RESOURCE_CONTENT += "<file alias=\"$$relativepath_out\">$$relativepath_out</file>"
}
RESOURCE_CONTENT += \
    "</qresource>" \
    "</RCC>"
write_file($$GENERATED_RESOURCE_FILE, RESOURCE_CONTENT)|error("Aborting.")
RESOURCES += $$GENERATED_RESOURCE_FILE

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
    settingsdialog.cpp \
    desktop.cpp

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
    settingsdialog.h \
    desktop.h \
    qreplytimeout.h \
    version.h

FORMS    += mainwindow.ui \
    newtabdialog.ui \
    settingsdialog.ui

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

qtPrepareTool(LUPDATE, lupdate)
ts-all.commands = cd $$PWD && $$LUPDATE -verbose MMMLauncher.pro
QMAKE_EXTRA_TARGETS += ts-all

qtPrepareTool(LCONVERT, lconvert)
LCONVERT+= -locations none
isEqual(QMAKE_DIR_SEP, /) {
 commit-ts.commands =  cd $$wd;  git add -N languages/*.ts &&  for f in `git diff-files —name-only languages/*.ts`; do  $$LCONVERT -i f -o f;  done;  git add languages/*_.ts && git commit
} else {
 commit-ts.commands =  cd $$wd &&  git add -N languages/*.ts &&  for /f usebackq %%f in (`git diff-files —name-only — languages/*.ts`) do  $$LCONVERT -i %f -o%f $$escape_expand(nt)  cd $$wd && git add languages/*_.ts && git commit
}
QMAKE_EXTRA_TARGETS += commit-ts
