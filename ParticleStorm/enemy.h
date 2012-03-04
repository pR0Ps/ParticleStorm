#ifndef ENEMY_H
#define ENEMY_H

#include "gameobject.h"

class Enemy : public GameObject
{
public:
    Enemy();

    void draw() const;
    void update();
    void applyForce(double x, double y, double mag);

    //initialize an instance of an enemy
    void initialize(double hlth, double spd, double dmg);

    //move from current position to given position
    void moveLine(double x, double y);

    //for every hit/heal to the enemy
    void decHealth(double dam);
    void incHealth(double heal);

private:
    double health;
    double speed;
    double damage;
};

#endif // ENEMY_H
