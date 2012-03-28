#ifndef POWERUP_H
#define POWERUP_H

#include "gameobject.h"
#include "objectmanager.h"

class Powerup : public GameObject
{
public:
    Powerup();

    static const float AIR_RESIST = 1;
    static const int MAX_ROTATION_SPD = 900;
    static const int MIN_ROTATION_SPD = 540;
    static const double INITIAL_TTL = 10;
    static const int NUM_LIGHTNING = 5;
    static const double COLLECT_RING_SIZE = .8;
    static const double VALUE_RATIO = 0.5;

    void startPowerup(int type, double x, double y, double x_vel, double y_vel, int value);

    void drawNoFade() const{}
    void drawFaded() const;
    void update(double deltaTime);
    void applyForce(double, double, double, double, double){}

    int getType() const {return type;}
    double getValue() const {return value;}

private:
    int type;
    int value;
    double spin;
    double angle;
    double ttl;
};

#endif // POWERUP_H
