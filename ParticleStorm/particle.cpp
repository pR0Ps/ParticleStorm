#include "particle.h"
#include "objectmanager.h"
#include "qmath.h"
#include <QColor>
#include "util.h"


Particle::Particle():GameObject(){
    double dt(1/30);
}

void Particle::update() {
    //update positions

    //x += x_vel * dt;
    //y += y_vel * dt;
}

void Particle::draw() const{
    glPushAttrib(GL_CURRENT_BIT);
    glColor3d(clr->red(), clr->green(), clr->blue());
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
    clr = ObjectManager::getInstance()->getParticleCol(Util::magnitude(x_vel, y_vel)/(float)MAX_PARTICLE_SPEED);
}
