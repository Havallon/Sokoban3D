#-------------------------------------------------
#
# Project created by QtCreator 2020-02-16T23:21:57
#
#-------------------------------------------------
QT += opengl
QT += multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Sokoban3D
TEMPLATE = app

CONFIG += c++11
SOURCES += main.cpp\
        menu.cpp

HEADERS  += menu.h

win32 {
    INCLUDEPATH += C:/freeglut-build-3.2.1/install/include
    LIBS += -LC:/freeglut-build-3.2.1/install/lib \
        -lfreeglut \
        -lOpengl32 \
        -lglu32
}
