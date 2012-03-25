#ifndef ENEMY_H
#define ENEMY_H

#include <string>
#include "gameobject.h"
#include "objectmanager.h"

class Enemy : public GameObject{
public:

    //redeclare in cpp file
    static const int MAX_DAMAGE = 10;


    Enemy();

    void update(double deltaTime);

    //move from current position to given position
    void move(double x, double y);

    //draw enemy
    void draw() const;

    //reset an enemy (almost identical to constructor)
    //y u no enemy type?
    void startEnemy(int t, double x, double y, double x_tar, double y_tar);

    //apply force (do nothing, but still need to override)
    void applyForce(double x, double y, double mag){}

    void pan(double x, double y);
    void die();

    int getDamage() const {return damage;}
    double getRadius() const {return radius;}

private:
    double speed;
    double damage;
    int type;
    double x_tar, y_tar;
    double radius;

    //shrapnel stuff
    int numShrapnel;
    double shrapnelLen;
};

#endif // ENEMY_H
