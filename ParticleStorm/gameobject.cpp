#include "gameobject.h"
#include "QtOpenGL"

//basic prototype of the GameObject class

GameObject::GameObject(){
    inUse = false;
    x = y = x_vel = y_vel = life = maxLife = 0;
    clr = NULL;
}

GameObject::~GameObject(){
    //clr is a pointer to a shared colour
    //delete in the class that holds the colour object, not here
    //seriously, don't, I just spent >1hr debugging this
    //delete clr;
}

void GameObject::pan(const double x, const double y){
    this->x += x;
    this->y += y;
}

void GameObject::die(){
    inUse = false;
}

void GameObject::setLife(const int n, const bool rel){
    if (rel)
        life = std::max(0, std::min (life + n, maxLife));
    else
        life = std::max(0, std::min (n, maxLife));
}
