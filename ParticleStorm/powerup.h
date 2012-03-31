#ifndef POWERUP_H
#define POWERUP_H

#include "gameobject.h"
#include "objectmanager.h"

class Powerup : public GameObject
{
public:
    Powerup();

    //declare in implementation
    static const float AIR_RESIST;
    static const int MAX_ROTATION_SPD;
    static const int MIN_ROTATION_SPD;
    static const double INITIAL_TTL;
    static const int NUM_LIGHTNING;
    static const double COLLECT_RING_SIZE;
    static const double VALUE_RATIO_HEALTH;
    static const double VALUE_RATIO_MANA;

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
