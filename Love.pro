#-------------------------------------------------
#
# Project created by QtCreator 2020-02-03T08:53:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Love
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
        widget.cpp

HEADERS += \
        widget.h

RESOURCES += \
    res.qrc



CONFIG(debug, debug|release) {
    WHICH_SPEC  = debug
    DESTDIR = ./bin/x86_debug
    OBJECTS_DIR = ./build/x86/debug
    MOC_DIR = ./build/x86/debug
    TARGET = LoveD

}
CONFIG(release, debug|release) {
    WHICH_SPEC  = release
    DESTDIR = ./bin/x86
    OBJECTS_DIR = ./build/x86/release
    MOC_DIR = ./build/x86/release
    TARGET = Love
}
