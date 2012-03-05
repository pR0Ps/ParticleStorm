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

    x += x_vel * dt;
    y += y_vel * dt;

    //update tail length?
}

void Particle::draw() const{
    double lastx = 0;
    double lasty = 0;
    Util::drawJaggedLine(x, y, lastx, lasty, clr);
}

void Particle::applyForce(double x, double y, double mag){
}

void Particle::die() {
    inUse = false;
}

void Particle::startParticle(double x1, double y1, double x2, double y2) {
    this->x = x1;
    this->y = y1;
    this->x_vel = (x1 - x2) / dt;
    this->y_vel = (y1 - y2) / dt;
    this->inUse = true;
}

void Particle::updateColour() {

    clr = ObjectManager::getInstance()->getParticleCol(Util::magnitude(x_vel, y_vel)/(double)MAX_PARTICLE_SPEED);
}
