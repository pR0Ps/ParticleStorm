#include "gameobject.h"

//basic prototype of the GameObject class

GameObject::GameObject(){
    inUse = false;
    x = y = x_vel = y_vel = life = maxLife = 0;
    clr = NULL;
}

GameObject::~GameObject(){
    delete clr;
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
