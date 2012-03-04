#ifndef SHRAPNEL_H
#define SHRAPNEL_H

#include "gameobject.h"

class Shrapnel : public GameObject
{
public:
    Shrapnel();

    void draw() const;
    void update();
    void applyForce(double x, double y, double mag);
};

#endif // SHRAPNEL_H
