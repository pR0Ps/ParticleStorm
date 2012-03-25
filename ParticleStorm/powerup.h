#ifndef POWERUP_H
#define POWERUP_H

#include "gameobject.h"
#include "objectmanager.h"

class Powerup : public GameObject
{
public:
    Powerup();

    static const float AIR_RESIST = 0.1;
    static const int MAX_ROTATION_SPD = 10;
    static const double INITIAL_TTL = 10;

    void startPowerup(int type, double x, double y, double x_vel, double y_vel, int value);

    void draw() const;
    void update(double deltaTime);
    void applyForce(double x, double y, double mag){}

    //Dies after a specific amount of time, or when player collides

private:
    int type;
    int value;
    double spin;
    double angle;
    double ttl;
};

#endif // POWERUP_H
