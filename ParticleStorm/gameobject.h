#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QColor>

class ObjectManager;

class GameObject {
public:
    GameObject();

    ~GameObject();

    //draw the object
    virtual void draw() const = 0;

    //pan the object with the screen
    virtual void pan(double x, double y);

    //update the object (pos from vel, AI, etc)
    virtual void update() = 0;

    //apply force to the object
    //x, y is the point that the force originated from
    //mag is the magnitude of the force
    virtual void applyForce(double x, double y, double mag) = 0;

    //kill the object
    virtual void die();

    //set
    void setInUse (bool inUse){this->inUse = inUse;}
    void setLife(int l, bool rel);
    void setMaxLife (int maxLife){this->maxLife = maxLife;}
    void setX(int x){this->x = x;}
    void setY(int y){this->y = y;}
    void setVelX(int x_vel){this->x_vel = x_vel;}
    void setVelY(int y_vel){this->y_vel = y_vel;}

    //get
    bool getInUse(){return inUse;}
    bool getLife(){return life;}
    bool getMaxLife(){return maxLife;}
    double getX(){return x;}
    double getY(){return y;}
    double getXVel(){return x_vel;}
    double getYVel(){return y_vel;}

protected:
    //properties
    double x, y;
    double x_vel, y_vel;
    int life, maxLife;
    const QColor *clr; //never need to modify it, just switch pointers
    bool inUse;
};

#endif // GAMEOBJECT_H
