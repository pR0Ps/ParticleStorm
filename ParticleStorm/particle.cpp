#include "particle.h"
#include "qmath.h"


Particle::Particle():GameObject(){



}

void Particle::update() {
    //update positions
    x += xVel;//*dt;
    y += yVel;//*dt;

    //update tail length?


}

void Particle::init() {
    mass = 1;
    xVel = 0;
    yVel = 0;

    updateColour();
}

void Particle::updateColour() {
    //set a new colour based on the new speed
}
