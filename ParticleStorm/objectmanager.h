#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include "gameobject.h"
#include <vector>

class ObjectManager{
public:
    ObjectManager();
    ~ObjectManager();

    //returns a pointer to the specified object
    GameObject* get(int n);

    //add an object to the manager
    virtual void addObject(GameObject *o);

    //apply to all the objects in the OM individually
    void draw();
    void pan(double x, double y);
    void update();
    void applyForce(double x, double y, double mag);

    //check for unused objects
    virtual void setUnused() = 0;

    //remove the unused objects (non-overridable)
    void destroyUnused();

    //look for a collision with other object(s) + handle result
    virtual void doCollision(ObjectManager *om) = 0;
    virtual void doCollision(GameObject *o) = 0;

    //get
    unsigned int getNumObjects();

private:
    //holds the objects
    std::vector<GameObject*> *objects;
};

#endif // OBJECTMANAGER_H
