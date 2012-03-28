#ifndef PARTICLE_H
#define PARTICLE_H

#include "gameobject.h"
#include <QColor>
#include "util.h"

class Particle : public GameObject
{
public:
    Particle();

    //constants (redeclare in implementation)
    static const int MAX_PARTICLE_SPEED = 1500;
    static const float SPEED_MULTIPLIER = 1;
    static const float AIR_RESIST = 1;
    static const float FORCE_DISSIPATION = 5;
    static const double FORCE_CUTOFF = 1500;
    static const int FORCE_EXERT = 100000;
    static const double INITITAL_TTL = 20;

    void drawNoFade() const{}
    void drawFaded() const;
    void update(double deltaTime);
    void die();
    void startParticle(double x, double y, double x_vel, double y_vel); //initializes a new particle, specifying the position
    float getSpeedPercent() const {return Util::magnitude(x_vel, y_vel)/(float)MAX_PARTICLE_SPEED;}

private:
    //internally change the particle's colour to match its speed
    void updateColour();
    double x_old, y_old;
    double ttl; //time to live
};

#endif // PARTICLE_H
