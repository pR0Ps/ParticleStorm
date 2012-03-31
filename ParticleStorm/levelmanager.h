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
        NONSTOP
    };

    //declare in implementation
    static const double TEXT_DISPLAY_TIME;
    static const double ENEMY_CHECK_RATE;
    static const int MAX_ENEMIES;
    static const int MIN_ENEMIES;
    static const int ENEMY_GROWTH;
    static const int ENEMY_SPAWN_DIST;

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

    //flags
    bool levelDone;
    bool enemiesStarted;

    //start the enemies
    void startEnemies();
    QPoint randomOffscreenPos();
};

#endif // LEVELMANAGER_H
