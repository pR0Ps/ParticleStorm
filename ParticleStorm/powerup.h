#ifndef POWERUP_H
#define POWERUP_H

#include "gameobject.h"
#include "objectmanager.h"

class Powerup : public GameObject
{
private:
    int type;
    double dt;
public:
    Powerup();

    static const float AIR_RESIST = 0.1;

    void startPowerup(int type, double x, double y, double x_vel, double y_vel);

    void draw() const;
    void update();
    void applyForce(double x, double y, double mag);

    //Dies after a specific amount of time, or when player collides
    void die();

};

#endif // POWERUP_H
