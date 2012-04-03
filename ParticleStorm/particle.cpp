#include "particle.h"
#include "resourcemanager.h"
#include "gameengine.h"
#include "util.h"
#include <iostream>
#include "qmath.h"

const int Particle::MAX_PARTICLE_SPEED = 1500;
const float Particle::SPEED_MULTIPLIER = 1;
const float Particle::AIR_RESIST = 1;
const float Particle::FORCE_DISSIPATION = 5;
const double Particle::FORCE_CUTOFF = 1500;
const int Particle::FORCE_EXERT = 100000;
const double Particle::INITITAL_TTL = 20;

Particle::Particle():GameObject(){

}

void Particle::update(double deltaTime) {

    //limit speed
    if(Util::magnitude(x_vel,y_vel) > MAX_PARTICLE_SPEED) {
        double angle = Util::atand(y_vel,x_vel);
        x_vel = MAX_PARTICLE_SPEED*Util::cosd(angle);
        y_vel = MAX_PARTICLE_SPEED*Util::sind(angle);
    }

    //apply air resistance
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

void Particle::drawFaded() const{
    //'tail' should take into account current speed
    //should be roughly the same location where the particle was last frame
    Util::drawLine(x, y, x_old, y_old, clr);
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
