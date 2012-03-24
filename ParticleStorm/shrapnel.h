#ifndef SHRAPNEL_H
#define SHRAPNEL_H

#include "gameobject.h"

class Shrapnel : public GameObject
{
public:
   static const int SHRAPNEL_TIME = 150;

    Shrapnel();

    void draw() const;
    void update(double deltaTime);
    void applyForce(double x, double y, double mag);
    void makeShrapnel(double x1, double y1);
};

#endif // SHRAPNEL_H
