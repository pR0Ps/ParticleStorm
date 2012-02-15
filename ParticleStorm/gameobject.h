#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QColor>
#include "objectmanager.h"

class GameObject {
public:
    explicit GameObject();
    bool isActive();
    void draw();
    void update();
    void modHealth(int n);
    void applyForce(double x, double y, double mag);
    void die();
    void doCollision(const ObjectManager& om);
    void doCollision(const GameObject ob);

private:
    double x, y;
    double x_vel, y_vel;
    int life;
    QColor clr;
};

#endif // GAMEOBJECT_H
