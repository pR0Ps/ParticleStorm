#include "gameobject.h"

GameObject::GameObject(ObjectManager *manager){
    setManager(manager);
    this->init();
}

GameObject::GameObject(){
    this->init();
}

void GameObject::init(){
    this->inUse = true;
}

void GameObject::pan(double x, double y){
    this->x += x;
    this->y += y;
}

void GameObject::die(){
    this->inUse = false;
}

//set
void GameObject::setManager(ObjectManager *om){
    this->manager = om;
}

void GameObject::setLife(int n, bool rel){
    if (rel){
        this->life += n;
    }
    else{
        this->life = n;
    }
}

//get
bool GameObject::getInUse(){
    return inUse;
}

bool GameObject::getLife(){
    return this->life;
}

double GameObject::getX(){
    return this->x;
}

double GameObject::getY(){
    return this->y;
}


