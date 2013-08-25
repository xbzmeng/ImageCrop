#-------------------------------------------------
#
# Project created by QtCreator 2013-08-24T11:15:18
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ImageCrop
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    settingdialog.cpp \
    mainview.cpp \
    resizedialog.cpp

HEADERS  += mainwindow.h \
    settingdialog.h \
    mainview.h \
    resizedialog.h

FORMS    += mainwindow.ui \
    settingdialog.ui \
    resizedialog.ui

RESOURCES += \
    ImageCrop.qrc

RC_FILE += \
    ImageCrop.rc

TRANSLATIONS += zh_CN.ts
