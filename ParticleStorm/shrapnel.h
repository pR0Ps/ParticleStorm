#ifndef SHRAPNEL_H
#define SHRAPNEL_H

#include "gameobject.h"

class Shrapnel : public GameObject
{
public:
    //redeclare in cpp file
    static const int MAX_SHRAPNEL_SPEED;
    static const int INITIAL_PUSH_MIN;
    static const int INITIAL_PUSH_MAX;
    static const int MAX_ROTATION_SPD;
    static const double INITIAL_TTL;
    static const int DECREMENT_SPEED;
    Shrapnel();

    void drawNoFade() const{}
    void drawFaded() const;
    void step(double deltaTime);
    void startShrapnel(double x, double y, double x_vel, double y_vel, double len, const Util::Color *clr);

private:
    double len; //length of shrapnel
    double spin; //how fast it spins
    double angle; //current angle
    double ttl;
};

#endif // SHRAPNEL_H
