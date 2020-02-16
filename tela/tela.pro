#-------------------------------------------------
#
# Project created by QtCreator 2020-02-14T23:16:04
#
#-------------------------------------------------

QT += opengl
QT += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tela
TEMPLATE = app

CONFIG += c++11
SOURCES += main.cpp\
        tela.cpp

HEADERS  += tela.h

win32 {
    INCLUDEPATH += C:/freeglut-build-3.2.1/install/include
    LIBS += -LC:/freeglut-build-3.2.1/install/lib \
        -lfreeglut \
        -lOpengl32 \
        -lglu32
}

DISTFILES +=
