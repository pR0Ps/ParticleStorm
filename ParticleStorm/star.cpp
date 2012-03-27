#include "particle.h"
#include "objectmanager.h"
#include "gameengine.h"
#include "util.h"
#include "star.h"

const float Star::FORCE_DISSIPATION;
const int Star::FORCE_EXERT;
const double Star::FORCE_CUTOFF;
const int Star::MAX_DIST;

Star::Star():GameObject(){
    //init the star
    inUse = true;
    x = x_old = qrand() % GameEngine::MAX_X;
    y = y_old = qrand() % GameEngine::MAX_Y;
    clr = ResourceManager::getInstance()->getColour(ResourceManager::WHITE);

    dist = qrand() % 3 == 1 ? 2 : qrand() % 3 == 1 ? MAX_DIST : Util::randInt(2, MAX_DIST);
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
    if (this->x > GameEngine::MAX_X){
        this->x -= GameEngine::MAX_X;
        this->x_old -= GameEngine::MAX_X;
    }
    else if (this->x < 0){
        this->x = GameEngine::MAX_X + x;
        this->x_old = GameEngine::MAX_X + x_old;
    }
    if (this->y > GameEngine::MAX_Y){
        this->y -= GameEngine::MAX_Y;
        this->y_old -= GameEngine::MAX_Y;
    }
    else if (this->y < 0){
        this->y = GameEngine::MAX_Y + y;
        this->y_old = GameEngine::MAX_Y + y_old;
    }

}

void Star::pan(double x, double y){
    this->x += x / dist;
    this->y += y / dist;
}

void Star::applyForce(double x, double y, double mag, double range){
    double dist = Util::distance(this->x,this->y,x,y);

    //out of range
    if (dist > range) return;

    if(dist == 0) {
        dist = 0.0001; //avoiding a div by 0 error in the next step
    }
    x_vel += (this->x - x) * mag / ((dist * dist) * FORCE_DISSIPATION * this->dist);
    y_vel += (this->y - y) * mag / ((dist * dist) * FORCE_DISSIPATION * this->dist);
}

void Star::drawNoFade() const{
    Util::drawLine(x, y, x_old, y_old, clr);
}
