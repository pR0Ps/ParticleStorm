#include "particle.h"
#include "objectmanager.h"
#include "gameengine.h"
#include "util.h"
#include "star.h"

const float Star::FORCE_DISSIPATION;
const int Star::EXTRA_BOUNDS;
const int Star::FORCE_EXERT;
const double Star::FORCE_CUTOFF;
const int Star::MAX_DIST;

Star::Star():GameObject(){
    //init the star
    inUse = true;
    x = x_old = qrand() % (GameEngine::MAX_X + EXTRA_BOUNDS*2) - EXTRA_BOUNDS;
    y = y_old = qrand() % (GameEngine::MAX_Y + EXTRA_BOUNDS*2) - EXTRA_BOUNDS;
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

}

void Star::pan(double x, double y){
    this->x += x / dist;
    this->y += y / dist;
}

void Star::applyForce(double x, double y, double mag){
    double dist = Util::distance(this->x,this->y,x,y);

    if (dist > FORCE_CUTOFF) return;

    if(dist == 0) {
        dist = 0.0001; //avoiding a div by 0 error in the next step
    }
    x_vel += (this->x - x) * mag / ((dist * dist) * FORCE_DISSIPATION * this->dist);
    y_vel += (this->y - y) * mag / ((dist * dist) * FORCE_DISSIPATION * this->dist);
}

void Star::draw() const{
    Util::drawLine(x, y, x_old, y_old, clr);
}
