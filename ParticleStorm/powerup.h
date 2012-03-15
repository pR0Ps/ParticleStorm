#ifndef POWERUP_H
#define POWERUP_H

#include "gameobject.h"
#include "objectmanager.h"

class Powerup : public GameObject
{
private:
    int type;
public:
    Powerup();

    void startPowerup(int type, double x, double y, double x_vel, double y_vel);

    void draw() const;
    void update();
    void applyForce(double x, double y, double mag);
};

#endif // POWERUP_H
