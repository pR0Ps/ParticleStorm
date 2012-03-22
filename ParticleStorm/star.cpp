#include "particle.h"
#include "objectmanager.h"
#include "gameengine.h"
#include "util.h"
#include "star.h"

const float Star::FORCE_DISSIPATION;
const int Star::EXTRA_BOUNDS;
const int Star::FORCE_EXERT;
const int Star::MAX_DIST;

Star::Star():GameObject(){
    //init the star
    inUse = true;
    x = x_old = qrand() % (GameEngine::MAX_X + EXTRA_BOUNDS*2) - EXTRA_BOUNDS;
    y = y_old = qrand() % (GameEngine::MAX_Y + EXTRA_BOUNDS*2) - EXTRA_BOUNDS;
    clr = ResourceManager::getInstance()->getColour(ResourceManager::WHITE);

    dist = qrand() % 3 == 1 ? 2 : qrand() % 3 == 1 ? MAX_DIST : qrand() % (MAX_DIST-2) + 2;
}

void Star::update(double deltaTime){
    //update velocity
    x_vel -= 2 * x_vel * deltaTime;
    y_vel -= 2 * y_vel * deltaTime;

    //move star
    x_old = x;
    y_old = y;
    x += x_vel * deltaTime;
    y += y_vel * deltaTime;

    //wrap the star around when panned offscreen
    if (this->x > GameEngine::MAX_X + EXTRA_BOUNDS){
        this->x = -EXTRA_BOUNDS;
        this->x_old = -EXTRA_BOUNDS;
    }
    else if (this->x < -EXTRA_BOUNDS){
        this->x = GameEngine::MAX_X + EXTRA_BOUNDS;
        this->x_old = GameEngine::MAX_X + EXTRA_BOUNDS;
    }
    if (this->y > GameEngine::MAX_Y + EXTRA_BOUNDS){
        this->y = -EXTRA_BOUNDS;
        this->y_old = -EXTRA_BOUNDS;
    }
    else if (this->y < -EXTRA_BOUNDS){
        this->y = GameEngine::MAX_Y + EXTRA_BOUNDS;
        this->y_old = GameEngine::MAX_Y + EXTRA_BOUNDS;
    }

    /*
    //foreground stars
    //  if (array[1-100])
    if(count<100){
    //if(x_pos<100||x_pos>800 && y_pos<100||y_pos>800)// is it on the edge
        if((x<100 || x>800) && (y<100 || y>800)){
        x_vel -= FOREGROUND_SPEED_EDGE*x_vel*deltaTime;
        y_vel -= FOREGROUND_SPEED_EDGE*y_vel*deltaTime;
        //positions
        x += x_vel *SPEED_MULTIPLIER*deltaTime;
        y += y_vel *SPEED_MULTIPLIER*deltaTime;
        }

    //else
    else{
        x_vel -= FOREGROUND_SPEED_CENTER*x_vel*deltaTime;
        y_vel -= FOREGROUND_SPEED_CENTER*y_vel*deltaTime;
        //positions
        x += x_vel *SPEED_MULTIPLIER*deltaTime;
        y += y_vel *SPEED_MULTIPLIER*deltaTime;
    }
}
  /////////////////////////////////////////
    //speed of background stars
    else{
         //if(x_pos<100||x_pos>800 && y_pos<100||y_pos>800)// is it on the edge
        if((x<100||x>800) && (y<100||y>100)){
            x_vel -= FOREGROUND_SPEED_EDGE*x_vel*deltaTime;
            y_vel -= FOREGROUND_SPEED_EDGE*y_vel*deltaTime;
            //positions
            x += x_vel *SPEED_MULTIPLIER*deltaTime;
            y += y_vel *SPEED_MULTIPLIER*deltaTime;
        }
            //else
            else {
            x_vel -= FOREGROUND_SPEED_CENTER*x_vel*deltaTime;
            y_vel -= FOREGROUND_SPEED_CENTER*y_vel*deltaTime;
            //positions
            x += x_vel *SPEED_MULTIPLIER*deltaTime;
            y += y_vel *SPEED_MULTIPLIER*deltaTime;
        }

    }
    if(count<250){
    count++;}
    else{count=0;}
    */
}

void Star::pan(double x, double y){
    this->x += x / dist;
    this->y += y / dist;
}

void Star::applyForce(double x, double y, double mag){
    double dist = Util::distance(this->x,this->y,x,y);
    if(dist == 0) {
        dist = 0.0001; //avoiding a div by 0 error in the next step
    }
    x_vel += (this->x - x) * mag / ((dist * dist) * FORCE_DISSIPATION * this->dist);
    y_vel += (this->y - y) * mag / ((dist * dist) * FORCE_DISSIPATION * this->dist);
}

void Star::draw() const{
    Util::drawLine(x, y, x_old, y_old, clr);
}
