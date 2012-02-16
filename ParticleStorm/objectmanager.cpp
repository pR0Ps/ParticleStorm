#include "objectmanager.h"

ObjectManager::ObjectManager(){}


GameObject* ObjectManager::get(int n){
    return objects->at(n);
}

unsigned int ObjectManager::getNumObjects(){
    return objects->size();
}

void ObjectManager::addObject(GameObject *o){
    o->setManager(this);
    objects->push_back(o);
}

void ObjectManager::draw(){
    for (unsigned int i = 0 ; i < objects->size() ; i++){
        objects->at(i)->draw();
    }
}

void ObjectManager::pan(double x, double y){
    for (unsigned int i = 0 ; i < objects->size() ; i++){
        objects->at(i)->pan(x, y);
    }
}

void ObjectManager::update(){
    for (unsigned int i = 0 ; i < objects->size() ; i++){
        objects->at(i)->update();
    }
}

void ObjectManager::applyForce(double x, double y, double mag){
    for (unsigned int i = 0 ; i < objects->size() ; i++){
        objects->at(i)->applyForce(x, y, mag);
    }
}

void ObjectManager::destroyUnused(){
    for(unsigned int i = 1; i < objects->size(); i++){
        if(!objects->at(i)->getInUse()){
            objects->erase(objects->begin() + i);
            i--;
        }
    }
}
