#-------------------------------------------------
#
# Project created by QtCreator 2023-12-01T16:24:16
#
#-------------------------------------------------

QT       += core gui multimedia
QT  += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RGRcld
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
        notemanager.cpp \
        addnotedialog.cpp \
        noteeditor.cpp \
        reminderdialog.cpp \
    popup.cpp \
    aboutme.cpp \
    searchdialog.cpp

HEADERS += \
        mainwindow.h \
        notemanager.h \
        addnotedialog.h \
        noteeditor.h \
        reminderdialog.h \
    popup.h \
    aboutme.h \
    searchdialog.h

FORMS += \
        mainwindow.ui
