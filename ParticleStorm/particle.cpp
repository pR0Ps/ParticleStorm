#include "particle.h"
#include "qmath.h"


Particle::Particle():GameObject(){



}

Particle::update(const double dt) {
    //update positions
    x += xVel*dt;
    y += yVel*dt;

    //update tail length?


}

Particle::init() {
    mass = 1;
    xVel = 0;
    yVel = 0;

    updateColour();
}

Particle::updateColour() {
    //set a new colour based on the new speed
}
