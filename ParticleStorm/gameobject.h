#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <QColor>

class ObjectManager;

class GameObject {
public:
    GameObject(ObjectManager *manager);
    GameObject();
    ~GameObject();

    //draw the object
    virtual void draw() = 0;

    //pan the object with the screen
    void pan(double x, double y);

    //update the object (pos from vel, AI, etc)
    virtual void update() = 0;

    //apply force to the object
    //x, y is the point that the force originated from
    //mag is the magnitude of the force
    virtual void applyForce(double x, double y, double mag) = 0;

    //kill the object (make sure to set inUse = false so it gets removed)
    virtual void die();

    //look for a collision with another object + handle result
    virtual void doCollision(GameObject *ob) = 0;

    //set
    void setManager(ObjectManager *om);
    void setLife(int n, bool rel);

    //get
    bool getInUse(){return inUse;}
    bool getLife(){return life;}
    double getX(){return x;}
    double getY(){return y;}

protected:
    //properties
    double x, y;
    double x_vel, y_vel;
    int life;
    QColor *colour;
    bool inUse;
    ObjectManager *manager;

private:
    //only called via the constructor
    virtual void init();
};

#endif // GAMEOBJECT_H
