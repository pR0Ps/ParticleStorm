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
    void applyForce(double, double, double);
    void die();
    void draw() const;
    void startParticle(double, double, double, double); //initializes a new particle, specifying the position

private:
    //internally change the particle's colour to match its speed

    void updateColour();

    double dt;
    double x,y;
    double xVel,yVel;
    double xtail,ytail;
    QColor clr;

};

#endif // PARTICLE_H
