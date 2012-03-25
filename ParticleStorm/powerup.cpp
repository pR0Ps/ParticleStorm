#include "powerup.h"
#include "gameengine.h"

const float Powerup::AIR_RESIST;
const int Powerup::MAX_ROTATION_SPD;
const double Powerup::INITIAL_TTL;

Powerup::Powerup():GameObject(){
    //do nothing
}

void Powerup::startPowerup(int type, double x, double y, double x_vel, double y_vel, int value){
    this->type = type;
    this->value = value;
    this->x = x;
    this->y = y;
    this->x_vel = x_vel;
    this->y_vel = y_vel;
    this->ttl = INITIAL_TTL;
    this->inUse = true;
}

void Powerup::draw() const{

}

void Powerup::update(double deltaTime){

    x_vel -= AIR_RESIST * x_vel * deltaTime;
    y_vel -= AIR_RESIST * y_vel * deltaTime;


    //kill old powerups
    if (ttl <= 0)
        die();
    else
        this->ttl -= deltaTime;
}
