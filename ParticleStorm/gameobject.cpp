#include "gameobject.h"

//basic prototype of the GameObject class

GameObject::GameObject(ObjectManager *manager){
    setManager(manager);
    this->init();
}

GameObject::~GameObject(){
    delete colour;
}

GameObject::GameObject(){
    this->init();
}

void GameObject::init(){
    inUse = true;
}

void GameObject::pan(const double x, const double y){
    this->x += x;
    this->y += y;
}

void GameObject::die(){
    inUse = false;
}

//set
void GameObject::setManager(ObjectManager *om){
    manager = om;
}

void GameObject::setLife(const int n, const bool rel){
    if (rel) life += n;
    else life = n;
}


