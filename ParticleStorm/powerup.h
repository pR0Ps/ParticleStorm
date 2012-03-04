#ifndef POWERUP_H
#define POWERUP_H

#include "gameobject.h"

class Powerup : public GameObject
{
public:
    Powerup();

    void draw() const;
    void update();
    void applyForce(double x, double y, double mag);
};

#endif // POWERUP_H
