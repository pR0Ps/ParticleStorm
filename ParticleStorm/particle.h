#ifndef PARTICLE_H
#define PARTICLE_H

#include "gameobject.h"
#include <QColor>
#include "util.h"

class Particle : public GameObject
{
public:
    Particle();

    //constants
    static const int MAX_PARTICLE_SPEED = 100;

    void draw() const;
    void update();
    void applyForce(double x, double y, double mag);
    void die();
    void startParticle(double x, double y, double x_vel, double y_vel); //initializes a new particle, specifying the position

private:
    //internally change the particle's colour to match its speed

    void updateColour();

    double dt;
};

#endif // PARTICLE_H
