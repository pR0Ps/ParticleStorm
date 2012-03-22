#include <QDebug>
#include "math.h"

#include "objectmanager.h"

ObjectManager* ObjectManager::instance = NULL;

const int ObjectManager::MAX_PARTICLES;
const int ObjectManager::MAX_ENEMIES;
const int ObjectManager::MAX_POWERUPS;
const int ObjectManager::MAX_SHRAPNEL;
const int ObjectManager::MAX_STARS;

ObjectManager::ObjectManager(){
    if (instance){
        qDebug() << "Can't use more than one ObjectManager";
        return;
    }

    //create everything
    player = new Player();
    particles = new std::vector<GameObject*>;
    enemies = new std::vector<GameObject*>;
    powerups = new std::vector<GameObject*>;
    shrapnel = new std::vector<GameObject*>;
    stars = new std::vector<GameObject*>;

    //init objects
    initPool(PARTICLE, MAX_PARTICLES);
    initPool(ENEMY, MAX_ENEMIES);
    initPool(POWERUP, MAX_POWERUPS);
    initPool(SHRAPNEL, MAX_SHRAPNEL);
    initPool(STAR, MAX_STARS);

    //set initial particle
    cur_particle = 0;

    //set self-reference
    instance = this;
}

ObjectManager::~ObjectManager(){
    //remove everything
    delete player;

    //game objects
    while(!particles->empty()) delete particles->back(), particles->pop_back();
    delete particles;
    while(!enemies->empty()) delete enemies->back(), enemies->pop_back();
    delete enemies;
    while(!powerups->empty()) delete powerups->back(), powerups->pop_back();
    delete powerups;
    while(!shrapnel->empty()) delete shrapnel->back(), shrapnel->pop_back();
    delete shrapnel;
    while(!stars->empty()) delete stars->back(), stars->pop_back();
    delete stars;
}

//resets the object manager
void ObjectManager::reset(){
    //reset the player
    player->reset();

    //reset the particle counter
    cur_particle = 0;

    //reset the objects
    deactivateAll(PARTICLE);
    deactivateAll(ENEMY);
    deactivateAll(POWERUP);
    deactivateAll(SHRAPNEL);
    deactivateAll(STAR);
}

//returns the correct vector to operate on based on object type
std::vector<GameObject*>& ObjectManager::getVector(ObjectType t){
    if (t == PARTICLE)
        return *particles;
    else if (t == ENEMY)
        return *enemies;
    else if (t == POWERUP)
        return *powerups;
    else if (t == SHRAPNEL)
        return *shrapnel;
    else if (t == STAR)
        return *stars;
    else
        throw "bad enum type passed to getVector()";
}

//apply to all objects
void ObjectManager::draw(ObjectType t){
    if (t == PLAYER){
        player->draw();
    }
    else{
        std::vector<GameObject*> temp = getVector(t);
        for (unsigned int i = 0 ; i < temp.size() ; i++){
            if (temp.at(i)->getInUse()){
                temp.at(i)->draw();
            }
        }
    }
}
void ObjectManager::pan(ObjectType t, const double x, const double y){
    if (t == PLAYER){
        //cannot pan the player
        return;
    }
    else{
        std::vector<GameObject*> temp = getVector(t);
        for (unsigned int i = 0 ; i < temp.size() ; i++){
            if (temp.at(i)->getInUse()){
                temp.at(i)->pan(x, y);
            }
        }
    }
}
void ObjectManager::update(ObjectType t, double deltaTime){
    if (t == PLAYER){
        player->update(deltaTime);
    }
    else{
        std::vector<GameObject*> temp = getVector(t);
        for (unsigned int i = 0 ; i < temp.size() ; i++){
            if (temp.at(i)->getInUse()){
                temp.at(i)->update(deltaTime);
            }
        }
    }
}
void ObjectManager::applyForce(const ObjectType t, const double x, const double y, const double mag){
    if (t == PLAYER){
        //cannot apply a force to player
        return;
    }
    else{
        std::vector<GameObject*> temp = getVector(t);
        for (unsigned int i = 0 ; i < temp.size() ; i++){
            if (temp.at(i)->getInUse()){
                temp.at(i)->applyForce(x, y, mag);
            }
        }
    }
}

//allocate memory and add the required number of objects
//never need to allocate any more, just reuse the previous ones
void ObjectManager::initPool(ObjectType t, const unsigned int numObjects){
    if (t == PARTICLE){
        particles->reserve(numObjects);
        for (unsigned int i = 0 ; i < numObjects ; i++){
            particles->push_back(new Particle());
        }
    }
    else if (t == ENEMY){
        enemies->reserve(numObjects);
        for (unsigned int i = 0 ; i < numObjects ; i++){
            enemies->push_back(new Enemy());
        }
    }
    else if (t == POWERUP){
        powerups->reserve(numObjects);
        for (unsigned int i = 0 ; i < numObjects ; i++){
            powerups->push_back(new Powerup());
        }
    }
    else if (t == SHRAPNEL){
        shrapnel->reserve(numObjects);
        for (unsigned int i = 0 ; i < numObjects ; i++){
            shrapnel->push_back(new Shrapnel());
        }
    }
    else if (t == STAR){
        stars->reserve(numObjects);
        for (unsigned int i = 0 ; i < numObjects ; i++){
            stars->push_back(new Star());
        }
    }
}

//sets all the items as inactive
void ObjectManager::deactivateAll(ObjectType t){
    if (t == PLAYER){
        //player cannot be set as inactive
        return;
    }
    else{
        std::vector<GameObject*> temp = getVector(t);
        for (unsigned int i = 0 ; i < temp.size() ; i++){
            temp.at(i)->setInUse(false);
        }
    }
}

//returns the number of objects in use
unsigned int ObjectManager::getNumObjects(ObjectType t){
    if (t == PLAYER){
        return 1;
    }
    else{
        std::vector<GameObject*> temp = getVector(t);
        unsigned int cnt = 0;
        for (unsigned int i = 0 ; i < temp.size() ; i++){
            if (temp.at(i)->getInUse())
                cnt++;
        }
        return cnt;
    }
}

//return an unused object
//for now, if there aren't any free, it returns NULL
//this can be changed in the future to expand the object pool
GameObject* ObjectManager::getUnused(ObjectType t){
    if (t == PLAYER){
        //cannot get an unused player
        return NULL;
    }
    else if (t == PARTICLE){
        //special rules for particles (always reuse oldest)
        cur_particle = ++cur_particle % particles->size();
        return particles->at(cur_particle);
    }
    else{
        std::vector<GameObject*> temp = getVector(t);
        for (unsigned int i = 0 ; i < temp.size() ; i++){
            if (!temp.at(i)->getInUse()){
                return temp.at(i);
            }
        }
        return NULL;
    }
}

//collisions
void ObjectManager::doEnemyParticleCollisions(){

}

void ObjectManager::doPlayerEnemyCollisions(){

}

void ObjectManager::doPlayerPowerupCollisions(){

}

//spawning
void ObjectManager::spawnParticle(const double x, const double y, const double x_vel, const double y_vel){
    static_cast<Particle*>(getUnused(PARTICLE))->startParticle(x, y, x_vel, y_vel);
}

void ObjectManager::spawnParticle(const double x, const double y){
    const unsigned int angle = qrand() % 359;
    const int mag = qrand() % 120 / 10.0;
    spawnParticle(x, y, mag * Util::cosd(angle), mag * Util::sind(angle));
}

void ObjectManager::spawnPowerup (PowerupType t, double x, double y, double x_vel, double y_vel, int value){
    static_cast<Powerup*>(getUnused(POWERUP))->startPowerup(t, x, y, x_vel, y_vel, value);
}

void ObjectManager::spawnEnemy(EnemyType t, const double x, const double y, const double x_tar, const double y_tar){
    //will be elsewhere, storing here for now
    /*
    //determine start position at the edge of the screen
    int start = qrand()%4;
    if(start == 0){
        x = 0;
        y = qrand()%768;
    }
    else if(start == 1){
        x = 1024;
        y = qrand()%768;
    }
    else if(start == 2){
        x = qrand()%1024;
        y = 0;
    }
    else if(start == 3){
        x = qrand()%1024;
        y = 768;
    }
    */
    static_cast<Enemy*>(getUnused(ENEMY))->startEnemy(t, x, y, x_tar, y_tar);
}

void ObjectManager::spawnShrapnel(const double x, const double y, const double x_vel, const double y_vel, const double len, const QColor *clr){

}

//Enemy* ObjectManager::getClosestEnemy(const double x, const double y, const double min_dist){
//    return NULL;
//}
