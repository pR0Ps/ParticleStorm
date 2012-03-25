#ifndef SHRAPNEL_H
#define SHRAPNEL_H

#include "gameobject.h"

class Shrapnel : public GameObject
{
public:
    //redeclare in cpp file
    static const int MAX_SHRAPNEL_SPEED = 100;
    static const int INITIAL_PUSH = 100;
    static const int MAX_ROTATION_SPD = 20;
    static const double INITIAL_TTL = 1;
    static const int MAX_SHRAPNEL_LENGTH =10;
    static const int DECREMENT_SPEED = 5;
    Shrapnel();

    void draw() const;
    void update(double deltaTime);
    void applyForce(double x, double y, double mag);
    void startShrapnel(double x, double y, double x_vel, double y_vel, double len, const QColor *clr);

private:
    double len; //length of shrapnel
    double spin; //how fast it spins
    double angle; //current angle
    double ttl;
};

#endif // SHRAPNEL_H
