#include "shrapnel.h"
#include "resourcemanager.h"
#include "util.h"
#include "gameengine.h"
#include <time.h>

const int Shrapnel::SHRAPNEL_TIME;
Shrapnel::Shrapnel():GameObject(){

}

void Shrapnel::makeShrapnel(double x1, double y1){
    this->x=x1;
    this->y=y1;
    this->inUse=true;
    clr = ResourceManager::getInstance()->getColour(ResourceManager::WHITE);


}

void Shrapnel::draw() const{
    int xdraw=x+10;
    int ydraw=y+10;
    Util::drawLine(x, y, xdraw, ydraw, clr);
    }


void Shrapnel::update(double deltaTime){

    int count;

    if((count) > SHRAPNEL_TIME){
        inUse=false;
        count=0;
    }
    count++;
}

void Shrapnel::applyForce(double x, double y, double mag){

}

