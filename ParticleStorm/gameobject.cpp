#include "gameobject.h"
#include "util.h"
#include "QtOpenGL"

//basic prototype of the GameObject class

GameObject::GameObject(){
    inUse = false;
    x = y = x_vel = y_vel = life = maxLife = 0;
    radius = 0;
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

void GameObject::modLife(const double amt, const bool rel){
    if (rel)
        life = Util::max(0, Util::min (life + amt, maxLife));
    else
        life = Util::max(0, Util::min (amt, maxLife));

    if(life <= 0)
        this->die();
}
