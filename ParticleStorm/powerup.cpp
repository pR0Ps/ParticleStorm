#include "powerup.h"
#include "gameengine.h"

Powerup::Powerup():GameObject(){
    //do nothing
}

void Powerup::startPowerup(int type, double x, double y, double x_vel, double y_vel){
    this->type = type;
    //set stuff based on type here
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


}

void Powerup::applyForce(double x, double y, double mag){

}

void Powerup::die(){

}
