#include "particle.h"
#include "objectmanager.h"
#include "qmath.h"
#include <QColor>
#include "util.h"


Particle::Particle():GameObject(){

    QColor colour(0,0,0);
    double dt(1/30);

}

void Particle::update() {
    //update positions

    x += xVel*dt;
    y += yVel*dt;

    //update tail length?
}

void Particle::draw() const{
    if(inUse)
        Util::drawJaggedLine(x,y,xTail,yTail,colour);
}

void Particle::applyForce(double x, double y, double mag){
}

void Particle::die() {
    inUse = false;
}

void Particle::startParticle(double x1, double y1, double x2, double y2) {
    x = x1;
    y = y2;
    xVel = (x2-x1)/dt;
    yVel = (y2-y1)/dt;
    this->inUse = true;
    }

void Particle::updateColour() {

    clr = ObjectManager::getInstance()->getParticleCol(Util::magnitude(x_vel, y_vel)/(double)MAX_PARTICLE_SPEED);
}
