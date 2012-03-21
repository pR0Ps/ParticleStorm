#include "powerup.h"
#include "gameengine.h"

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
    this->inUse = true;
}

void Powerup::draw() const{

}

void Powerup::update(){

    dt = (1/(double)GameEngine::MAX_FPS);
    x_vel -= AIR_RESIST * x_vel * dt;
    y_vel -= AIR_RESIST * y_vel * dt;

}
