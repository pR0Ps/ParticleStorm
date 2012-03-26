#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include "objectmanager.h"
#include "gameengine.h"
#include "resourcemanager.h"

class LevelManager
{
public:
    LevelManager();

    //constants
    static const double INITIAL_TTL = 5;

    //starts and advances the level
    void startLevel(int lvl);
    void nextLevel();

    //updates the levelManager and polls isFinished
    void update(double deltaTime);

    void draw();

    //returns true if there are no enemies
    bool isFinished();

private:
    int currLvl;
    double ttl;
};

#endif // LEVELMANAGER_H
