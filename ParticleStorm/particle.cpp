#include "particle.h"
#include "objectmanager.h"
#include "gameengine.h"
#include "util.h"

#include "qmath.h"
#include <QColor>

const int Particle::MAX_PARTICLE_SPEED;
const float Particle::SPEED_MULTIPLIER;
const float Particle::AIR_RESIST;

Particle::Particle():GameObject(){

}

void Particle::update() {

    dt = (1/(double)GameEngine::MAX_FPS);

    //apply air resistance
    //x_vel += ((x_vel < 0) ? AIR_RESIST : -AIR_RESIST);
    //y_vel += (y_vel < 0) ? AIR_RESIST : -AIR_RESIST;

    x_vel -= AIR_RESIST * x_vel * dt;
    y_vel -= AIR_RESIST * y_vel * dt;
    //update positions
    x += x_vel * SPEED_MULTIPLIER * dt;
    y += y_vel * SPEED_MULTIPLIER * dt;
}

void Particle::draw() const{
    glPushAttrib(GL_CURRENT_BIT);
    glColor3d(clr->red(), clr->green(), clr->blue());

    //'tail' should take into account current speed
    //should be roughly the same location where the particle was last frame
    glBegin(GL_LINES);
        glVertex2d(x, y);
        glVertex2d(x + 10, y + 10);
    glEnd();
    glPopAttrib();
}

void Particle::applyForce(double x, double y, double mag){

    double dist = Util::distance(this->x,this->y,x,y);
    //calculating and updating x and y velocity using a 1/dist magnitude scaling
    x_vel += (this->x - x) * mag / ((dist * dist) * GameEngine::FORCE_DISSIPATION) * dt;
    y_vel += (this->y - y) * mag / ((dist * dist) * GameEngine::FORCE_DISSIPATION) * dt;
}

void Particle::die() {
    inUse = false;
}

void Particle::startParticle(double x1, double y1, double x_vel, double y_vel) {
    this->x = x1;
    this->y = y1;
    this->x_vel = x_vel;
    this->y_vel = y_vel;
    this->inUse = true;
    this->updateColour();
}

void Particle::updateColour() {
    clr = ResourceManager::getInstance()->getColourScale(Util::magnitude(x_vel, y_vel)/(float)MAX_PARTICLE_SPEED);
}
