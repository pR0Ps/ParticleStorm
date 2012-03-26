#ifndef LEVELMANAGER_H
#define LEVELMANAGER_H

#include "objectmanager.h"
#include "gameengine.h"
#include "resourcemanager.h"

class LevelManager
{
public:
    LevelManager();

    enum LevelType{
        ZEN,
        LEVELED,
        INFINITE
    };

    //constants
    static const double TEXT_DISPLAY_TIME = 5;
    static const double ENEMY_CHECK_RATE = 5;

    static const int MAX_ENEMIES = 50;
    static const int MIN_ENEMIES = 5;
    static const int ENEMY_GROWTH = 3;

    //starts the level
    void startLevel(LevelType t, int lvl);
    void nextLevel();

    //updates the levelManager and polls isFinished
    void update(double deltaTime);

    //draw the level manager stuff (text overlays)
    void drawNoFade() const;

    //returns true if there are no enemies
    bool levelFinished() const;

    //get the current level
    int getCurrentLevel() const {return currLvl;}

private:
    //state stuff
    LevelType currType;
    int currLvl;

    //timers
    double text_ttl;
    double enemyCheck_ttl;

    //finished flag
    bool levelDone;
};

#endif // LEVELMANAGER_H
