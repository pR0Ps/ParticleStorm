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
    gameobject.cpp \
    globals.cpp \
    player.cpp \
    star.cpp \
    particle.cpp \
    powerup.cpp \
    enemy.cpp \
    shrapnel.cpp \
    starmanager.cpp \
    particlemanager.cpp \
    powerupmanager.cpp \
    enemymanager.cpp \
    shrapnelmanager.cpp

HEADERS  += mainwindow.h \
    glcanvas.h \
    objectmanager.h \
    gameobject.h \
    globals.h \
    player.h \
    star.h \
    particle.h \
    powerup.h \
    enemy.h \
    shrapnel.h \
    starmanager.h \
    particlemanager.h \
    powerupmanager.h \
    enemymanager.h \
    shrapnelmanager.h

FORMS    += mainwindow.ui

RESOURCES += \
    Resources/resources.qrc
