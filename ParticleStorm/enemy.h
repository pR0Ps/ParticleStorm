#ifndef ENEMY_H
#define ENEMY_H

#include <string>
#include "gameobject.h"

class Enemy : public GameObject{
public:
    Enemy(string);

    void update(Player);

    //move from current position to given position
    void move(double, double);

    //for every hit/heal to the enemy
    void changeHealth(double);

    //draw enemy
    void draw();

    //reset an enemy (almost identical to constructor)
    void reset();

private:
    const double MAXHLTH;
    double health;
    double speed;
    double damage;
    double x,y;
    bool active;
    string type;
};

#endif // ENEMY_H
