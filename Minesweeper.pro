#-------------------------------------------------
#
# Project created by QtCreator 2020-06-11T18:04:57
#
#-------------------------------------------------

QT       += core gui
QT       += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Minesweeper
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

CONFIG += c++11

SOURCES += \
        cell.cpp \
        gameplay.cpp \
        main.cpp \
        mainwindow.cpp \
        mode_select.cpp \
        user_defined_input.cpp

HEADERS += \
        cell.h \
        gameplay.h \
        mainwindow.h \
        mode_select.h \
        user_defined_input.h

FORMS += \
        gameplay.ui \
        mainwindow.ui \
        mode_select.ui \
        user_defined_input.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    pic.qrc
