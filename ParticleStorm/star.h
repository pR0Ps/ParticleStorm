#ifndef STAR_H
#define STAR_H

#include <QColor>
#include "util.h"

#include "gameobject.h"

class Star : public GameObject
{
public:
    static const float FORCE_DISSIPATION = 25;
    static const int FORCE_EXERT = 5000;
    static const double FORCE_CUTOFF = 250;
    static const int MAX_DIST = 10;

    Star();

    void drawNoFade() const;
    void drawFaded() const{}
    void pan(double x, double y);
    void update(double deltaTime);
    void applyForce(double x, double y, double mag, double range, double dissipation);
private:
    double x_old, y_old;
    double dist;
};

#endif // STAR_H
