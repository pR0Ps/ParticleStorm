#ifndef ENEMY_H
#define ENEMY_H

#include <string>
#include "gameobject.h"
#include "objectmanager.h"

class Enemy : public GameObject{
public:

    //define in implementation
    static const int MAX_DAMAGE;
    static const int FORCE_DISSIPATION;
    static const double MAX_COLLISION_BUFFER_TIME;
    static const int MAX_ROTATION_SPD;
    static const int MIN_ROTATION_SPD;
    static const int OOB_LIMIT;
    static const int OOB_ALLOWANCE;
    static const int TANK_PARTICLE_FORCE;
    static const int TANK_STAR_FORCE;

    Enemy();

    void update(double deltaTime);

    //move from current position to given position
    void move(double x, double y);

    //draw enemy
    void drawNoFade() const;
    void drawFaded() const;

    //reset an enemy (almost identical to constructor)
    //y u no enemy type?
    void startEnemy(int type, double x, double y, double x_tar, double y_tar);

    void pan(double x, double y);
    void die();
    bool isImmune() const {return collisionBufferTime > 0;}
    int getDamage() const {return damage;}
    void setImmune() {collisionBufferTime = MAX_COLLISION_BUFFER_TIME;}

private:
    double speed;
    double damage;
    int type;
    double x_tar, y_tar;
    double collisionBufferTime;
    Enemy *currentEnemy;
    int points;

    //timer stuff
    double currTimer;
    double maxTimer;
    bool timerActive;
    bool moving;
    double playerX, playerY;

    //spin stuff
    double spin;
    double angle;

    void findDirection(double, double, double, double);

    //shrapnel stuff
    int numShrapnel;
    double shrapnelLen;
};

#endif // ENEMY_H
