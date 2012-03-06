#include "particle.h"
#include "objectmanager.h"
#include "gameengine.h"
#include "util.h"

#include "qmath.h"
#include <QColor>


Particle::Particle():GameObject(){

}

void Particle::update() {

    dt = (1/(double)GameEngine::MAX_FPS);

    //apply air resistance
    x_vel += (x_vel < 0) ? AIR_RESIST : -AIR_RESIST;
    y_vel += (y_vel < 0) ? AIR_RESIST : -AIR_RESIST;

    //update positions
    x += x_vel * SPEED_MULTIPLIER * dt;
    y += y_vel * SPEED_MULTIPLIER * dt;
}

void Particle::draw() const{
    glPushAttrib(GL_CURRENT_BIT);
    glColor3d(clr->red(), clr->green(), clr->blue());

    //'tail' should take into account current speed
    //should be roughy the same location where the particle was last frame
    glBegin(GL_LINES);
        glVertex2d(x, y);
        glVertex2d(x + 10, y + 10);
    glEnd();
    glPopAttrib();
}

void Particle::applyForce(double x, double y, double mag){
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
    clr = ResourceManager::getInstance()->getColour(Util::magnitude(x_vel, y_vel)/(float)MAX_PARTICLE_SPEED);
}
