#include "particle.h"
#include "resourcemanager.h"
#include "gameengine.h"
#include "util.h"
#include <iostream>
#include "qmath.h"
#include <QColor>

const int Particle::MAX_PARTICLE_SPEED;
const float Particle::SPEED_MULTIPLIER;
const float Particle::AIR_RESIST;
const float Particle::FORCE_DISSIPATION;
const int Particle::FORCE_EXERT;
const double Particle::INITITAL_TTL;

Particle::Particle():GameObject(){

}

void Particle::update(double deltaTime) {

    x_vel -= AIR_RESIST * x_vel * deltaTime;
    y_vel -= AIR_RESIST * y_vel * deltaTime;

    //update positions
    x_old = x;
    y_old = y;
    x += x_vel * SPEED_MULTIPLIER * deltaTime;
    y += y_vel * SPEED_MULTIPLIER * deltaTime;

    this->updateColour();

    //kill old particles
    if (ttl <= 0)
        die();
    else
        this->ttl -= deltaTime;

}

void Particle::draw() const{
    //'tail' should take into account current speed
    //should be roughly the same location where the particle was last frame
    Util::drawLine(x, y, x_old, y_old, clr);
}

void Particle::applyForce(double x, double y, double mag){

    double dist = Util::distance(this->x,this->y,x,y);
    //calculating and updating x and y velocity using a 1/dist magnitude scaling

    if(dist == 0) {
        dist = 0.0001; //avoiding a div by 0 error in the next step
    }

    x_vel += (this->x - x) * mag / ((dist * dist) * FORCE_DISSIPATION);
    y_vel += (this->y - y) * mag / ((dist * dist) * FORCE_DISSIPATION);

    if(Util::magnitude(x_vel,y_vel) > MAX_PARTICLE_SPEED) {
        double angle = Util::atand(y_vel,x_vel);
        x_vel = MAX_PARTICLE_SPEED*Util::cosd(angle);
        y_vel = MAX_PARTICLE_SPEED*Util::sind(angle);
    }
}

void Particle::die() {
    inUse = false;
}

void Particle::startParticle(double x1, double y1, double x_vel, double y_vel) {
    this->x = x1;
    this->y = y1;
    this->x_vel = x_vel;
    this->y_vel = y_vel;
    this->ttl = INITITAL_TTL;
    this->inUse = true;
    this->updateColour();
}

void Particle::updateColour() {
    clr = ResourceManager::getInstance()->getColourScale(getSpeedPercent());
}
