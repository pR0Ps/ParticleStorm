#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include "objectmanager.h"
#include "gameengine.h"
#include "mainwindow.h"
#include "resourcemanager.h"

class levelmanager
{
public:
    levelmanager();

    //starts the next level
    void startLevel();

    //returns true if there are no enemies
    bool isFinished();
};

#endif // LEVELMANAGER_H
