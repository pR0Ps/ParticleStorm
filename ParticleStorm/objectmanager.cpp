#include "objectmanager.h"

ObjectManager::ObjectManager(){
    objects = new std::vector<GameObject*>;
}

ObjectManager::~ObjectManager(){
    while(!objects->empty()) delete objects->back(), objects->pop_back();
    delete objects;
}

GameObject* ObjectManager::get(const int n){
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

void ObjectManager::pan(const double x, const double y){
    for (unsigned int i = 0 ; i < objects->size() ; i++){
        objects->at(i)->pan(x, y);
    }
}

void ObjectManager::update(){
    for (unsigned int i = 0 ; i < objects->size() ; i++){
        objects->at(i)->update();
    }
}

void ObjectManager::applyForce(const double x, const double y, const double mag){
    for (unsigned int i = 0 ; i < objects->size() ; i++){
        objects->at(i)->applyForce(x, y, mag);
    }
}

void ObjectManager::destroyUnused(){
    for(unsigned int i = 1; i < objects->size(); i++){
        GameObject *temp = objects->at(i);
        if(!temp->getInUse()){
            delete temp;
            objects->erase(objects->begin() + i);
            i--;
        }
    }
}
