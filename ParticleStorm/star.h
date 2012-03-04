#ifndef STAR_H
#define STAR_H

#include "gameobject.h"

class Star : public GameObject
{
public:
    Star();

    void draw() const;
    void update();
    void applyForce(double x, double y, double mag);
};

#endif // STAR_H
