#ifndef STAR_H
#define STAR_H

#include <QColor>
#include "util.h"

#include "gameobject.h"

class Star : public GameObject
{
public:
   //constants that will be used to control speed
    static const float FOREGROUND_SPEED_CENTER =0.5 ;
    static const float FOREGROUND_SPEED_EDGE = 0.8;
    static const float BACKGROUND_SPEED_CENTER = 0.3;
    static const float BACKGROUND_SPEED_EDGE = 0.4;
    static const float SCALE_FORCE_DISSIPATION = 10;
    static const float SPEED_MULTIPLIER =1; //just incase

    Star();

    void draw() const;
    void update();
    void applyForce(double x, double y, double mag);
private:
     int dt;

};

#endif // STAR_H
