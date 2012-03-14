#ifndef ENEMY_H
#define ENEMY_H

#include <string>
#include "gameobject.h"
#include "objectmanager.h"

class Enemy : public GameObject{
public:
    Enemy();

    void update();

    //move from current position to given position
    void move(double x, double y);

    //draw enemy
    void draw() const;

    //reset an enemy (almost identical to constructor)
    //y u no enemy type?
    void startEnemy(int t, double x, double y, double x_tar, double y_tar);

    //apply force (do nothing, but still need to override)
    void applyForce(double x, double y, double mag){}

private:
    double speed;
    double damage;
    bool active;
    int type;
};

#endif // ENEMY_H
