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
    objectmanager.cpp \
    gameobject.cpp \
    player.cpp \
    star.cpp \
    particle.cpp \
    powerup.cpp \
    enemy.cpp \
    shrapnel.cpp \
    util.cpp \
    gameengine.cpp \
    resourcemanager.cpp \
    levelmanager.cpp

HEADERS  += mainwindow.h \
    objectmanager.h \
    gameobject.h \
    player.h \
    star.h \
    particle.h \
    powerup.h \
    enemy.h \
    shrapnel.h \
    util.h \
    gameengine.h \
    resourcemanager.h \
    levelmanager.h

FORMS    += mainwindow.ui

RESOURCES += \
    Resources/resources.qrc
