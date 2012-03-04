#include "particle.h"
#include "objectmanager.h"
#include "util.h"

#include "qmath.h"


Particle::Particle():GameObject(){

}

void Particle::draw() const{

}

void Particle::update() {
    //update positions
    x += x_vel;//*dt;
    y += y_vel;//*dt;

    //update tail length?
}

void Particle::applyForce(double x, double y, double mag){

}

void Particle::startParticle(const double x, const double y, const double x_vel, const double y_vel, const QColor *clr){
    this->x = x;
    this->y = y;
    this->x_vel = x_vel;
    this->y_vel = y_vel;

    if (clr != NULL)
        this->clr = clr;
    else
        this->setColour();

    this->inUse = true;
}

void Particle::setColour() {
    //set a new colour based on the speed
    clr = ObjectManager::getInstance()->getParticleCol(Util::magnitude(x_vel, y_vel)/(double)MAX_PARTICLE_SPEED);
}
