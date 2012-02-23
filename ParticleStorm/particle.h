#ifndef PARTICLE_H
#define PARTICLE_H

#include "gameobject.h"

class Particle : public GameObject
{
public:
    Particle();
    void update();
    void applyForce(double x, double y); //don't need x, y, and mag (mag can be derived from x,y)



private:
    //internally change the particle's colour to match its speed
    void updateColour();
    void init();

    double mass;
    double x,y;
    double xVel,yVel;

};

#endif // PARTICLE_H
