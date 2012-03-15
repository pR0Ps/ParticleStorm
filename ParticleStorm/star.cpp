#include "particle.h"
#include "objectmanager.h"
#include "gameengine.h"
#include "util.h"
#include "star.h"

Star::Star():GameObject(){

}

int count = 0;

void Star::update(){
    dt = (1/(double)GameEngine::MAX_FPS);
    //foreground stars
    //  if (array[1-100])
    if(count<100){
    //if(x_pos<100||x_pos>800 && y_pos<100||y_pos>800)// is it on the edge
        if((x<100 || x>800) && (y<100 || y>800)){
        x_vel -= FOREGROUND_SPEED_EDGE*x_vel*dt;
        y_vel -= FOREGROUND_SPEED_EDGE*y_vel*dt;
        //positions
        x += x_vel *SPEED_MULTIPLIER*dt;
        y += y_vel *SPEED_MULTIPLIER*dt;
        }

    //else
    else{
        x_vel -= FOREGROUND_SPEED_CENTER*x_vel*dt;
        y_vel -= FOREGROUND_SPEED_CENTER*y_vel*dt;
        //positions
        x += x_vel *SPEED_MULTIPLIER*dt;
        y += y_vel *SPEED_MULTIPLIER*dt;
    }
}
  /////////////////////////////////////////
    //speed of background stars
    else{
         //if(x_pos<100||x_pos>800 && y_pos<100||y_pos>800)// is it on the edge
        if((x<100||x>800) && (y<100||y>100)){
            x_vel -= FOREGROUND_SPEED_EDGE*x_vel*dt;
            y_vel -= FOREGROUND_SPEED_EDGE*y_vel*dt;
            //positions
            x += x_vel *SPEED_MULTIPLIER*dt;
            y += y_vel *SPEED_MULTIPLIER*dt;
        }
            //else
            else {
            x_vel -= FOREGROUND_SPEED_CENTER*x_vel*dt;
            y_vel -= FOREGROUND_SPEED_CENTER*y_vel*dt;
            //positions
            x += x_vel *SPEED_MULTIPLIER*dt;
            y += y_vel *SPEED_MULTIPLIER*dt;
        }

    }
    if(count<250){
    count++;}
    else{count=0;}
}

void Star::applyForce(double x, double y, double mag){
   // if I understand this correctly, I should get the distance between the star and
   // player, and then apply a force to it. Force dissipation for this will be extremely small
   // by being scaled down quite a bit by SCALE_FORCE_DISSIPATION
    double dist = Util::distance(this->x,this->y,x,y);

    x_vel += (this->x- x) * mag / ((dist*dist)*(GameEngine::FORCE_DISSIPATION/Star::SCALE_FORCE_DISSIPATION)*dt);
    y_vel += (this->y- y) * mag / ((dist*dist)*(GameEngine::FORCE_DISSIPATION/Star::SCALE_FORCE_DISSIPATION)*dt);

}

void Star::draw() const{
    double size_box_x= x+1;
    double size_box_y= y+1;

    Util::drawBox(x, y, size_box_x, size_box_y, ResourceManager::getInstance()->getColour(ResourceManager::WHITE));

}
