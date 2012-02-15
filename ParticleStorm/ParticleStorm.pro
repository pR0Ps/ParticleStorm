#-------------------------------------------------
#
# Project created by QtCreator 2012-02-14T18:48:31
#
#-------------------------------------------------

QT       += core gui opengl

TARGET = ParticleStorm
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    glcanvas.cpp \
    objectmanager.cpp \
    gameobject.cpp

HEADERS  += mainwindow.h \
    glcanvas.h \
    objectmanager.h \
    gameobject.h

FORMS    += mainwindow.ui

RESOURCES += \
    Resources/resources.qrc
