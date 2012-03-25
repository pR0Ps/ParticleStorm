#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include "objectmanager.h"
#include "gameengine.h"
#include "mainwindow.h"
#include "resourcemanager.h"

class LevelManager
{
public:
    LevelManager();

    //starts the next level
    void startLevel();

    //returns true if there are no enemies
    bool isFinished();

private:
    int currLvl;
};

#endif // LEVELMANAGER_H
