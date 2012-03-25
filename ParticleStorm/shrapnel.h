#ifndef SHRAPNEL_H
#define SHRAPNEL_H

#include "gameobject.h"

class Shrapnel : public GameObject
{
public:
   static const int SHRAPNEL_TIME = 150;
   static const int MAX_SHRAPNEL_SPEED = 100;
   static const int INITIAL_PUSH = 100;
   static const int MAX_ROTATION_SPD = 40;

    Shrapnel();

    void draw() const;
    void update(double deltaTime);
    void applyForce(double x, double y, double mag);
    void startShrapnel(double x, double y, double x_vel, double y_vel, double len, const QColor *clr);

private:
    double len; //length of shrapnel
    double spin; //how fast it spins
    double angle; //current angle
};

#endif // SHRAPNEL_H
