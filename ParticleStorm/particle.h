#ifndef PARTICLE_H
#define PARTICLE_H

#include "gameobject.h"

class Particle : public GameObject
{
public:
    Particle();

    //constants
    static const int MAX_PARTICLE_SPEED = 100;

    void draw() const;
    void update();
    void applyForce(double x, double y, double mag);

    //sets the needed information and starts the particle
    void startParticle(double x, double y, double x_vel, double y_vel, const QColor *clr = NULL);



private:
    //internally change the particle's colour to match its speed
    void setColour();
    const QColor *clr; //never need to modify it, just switch pointers
};

#endif // PARTICLE_H
