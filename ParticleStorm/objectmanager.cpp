#include <QDebug>
#include "math.h"

#include "objectmanager.h"

ObjectManager* ObjectManager::instance = NULL;

ObjectManager::ObjectManager(){
    if (instance){
        qDebug() << "Can't use more than one ObjectManager";
        return;
    }

    //create everything
    player = new Player();
    particles = new std::vector<Particle*>;
    enemies = new std::vector<Enemy*>;
    powerups = new std::vector<Powerup*>;
    shrapnel = new std::vector<Shrapnel*>;
    stars = new std::vector<Star*>;

    //init objects
    init(PARTICLE, MAX_PARTICLES);
    init(ENEMY, MAX_ENEMIES);
    init(POWERUP, MAX_POWERUPS);
    init(SHRAPNEL, MAX_SHRAPNEL);
    init(STAR, MAX_STARS);

    //init colour arrays
    particleCol = new std::vector<const QColor*>;
    float per;
    for (unsigned int i = 0 ; i < NUM_PARTICLE_COLOURS ; i++){
        //(gradiant from blue -> green -> red, like UI mockup)
        per = i/(float)NUM_PARTICLE_COLOURS;
        //hard one-liner is hard
        particleCol->push_back(new QColor(
            (per <= .1) ? ((255/2.0)-(per/.1)*(255/2.0)) : ((per < .5) ? (0) : ((per <= .7) ? (((per-.5)/.2)*255) : (255))),
            (per <= .1) ? (0) : ((per <= .3) ? (((per-.1)/(.2))*255) : ((per <= .7) ? (255) : (255-((per-.7)/(.3))*255))),
            (per <= .3) ? (255) : (per <= .5) ? (255-((per-.3)/(.2))*255) : (0)
        ));
    }

    //set initial particle
    cur_particle = 0;

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

    //colours
    while(!particleCol->empty()) delete particleCol->back(), particleCol->pop_back();
    delete particleCol;
}

//GAH, Y U NO WORK?
/*
std::vector<GameObject*>& ObjectManager::getVector(ObjectTypes t){
    if (t == PARTICLE)
        return &particles;
    else if (t == ENEMY)
        return &enemies;
    else if (t == POWERUP)
        return &powerups;
    else if (t == SHRAPNEL)
        return &shrapnel;
    else if (t == STAR)
        return &stars;
    else
        return NULL;
}
*/

//apply to all objects
void ObjectManager::draw(ObjectType t){
    //once getVector is working, this will get a lot cleaner
    if (t == PARTICLE){
        for (unsigned int i = 0 ; i < particles->size() ; i++){
            if (particles->at(i)->getInUse())
                particles->at(i)->draw();
        }
    }
    else if (t == ENEMY){
        for (unsigned int i = 0 ; i < enemies->size() ; i++){
            if (enemies->at(i)->getInUse())
                enemies->at(i)->draw();
        }
    }
    else if (t == POWERUP){
        for (unsigned int i = 0 ; i < powerups->size() ; i++){
            if (powerups->at(i)->getInUse())
                powerups->at(i)->draw();
        }
    }
    else if (t == SHRAPNEL){
        for (unsigned int i = 0 ; i < shrapnel->size() ; i++){
            if (enemies->at(i)->getInUse())
            shrapnel->at(i)->draw();
        }
    }
    else if (t == STAR){
        for (unsigned int i = 0 ; i < stars->size() ; i++){
            if (stars->at(i)->getInUse())
                stars->at(i)->draw();
        }
    }
}
void ObjectManager::pan(ObjectType t, const double x, const double y){
    //once getVector is working, this will get a lot cleaner
    if (t == PARTICLE){
        for (unsigned int i = 0 ; i < particles->size() ; i++){
            if (particles->at(i)->getInUse())
                particles->at(i)->pan(x, y);
        }
    }
    else if (t == ENEMY){
        for (unsigned int i = 0 ; i < enemies->size() ; i++){
            if (enemies->at(i)->getInUse())
                enemies->at(i)->pan(x, y);
        }
    }
    else if (t == POWERUP){
        for (unsigned int i = 0 ; i < powerups->size() ; i++){
            if (powerups->at(i)->getInUse())
                powerups->at(i)->pan(x, y);
        }
    }
    else if (t == SHRAPNEL){
        for (unsigned int i = 0 ; i < shrapnel->size() ; i++){
            if (shrapnel->at(i)->getInUse())
                shrapnel->at(i)->pan(x, y);
        }
    }
    else if (t == STAR){
        for (unsigned int i = 0 ; i < stars->size() ; i++){
            if (stars->at(i)->getInUse())
                stars->at(i)->pan(x, y);
        }
    }
}
void ObjectManager::update(ObjectType t){
    //once getVector is working, this will get a lot cleaner
    if (t == PARTICLE){
        for (unsigned int i = 0 ; i < particles->size() ; i++){
            if (particles->at(i)->getInUse())
                particles->at(i)->update();
        }
    }
    else if (t == ENEMY){
        for (unsigned int i = 0 ; i < enemies->size() ; i++){
            if (enemies->at(i)->getInUse())
                enemies->at(i)->update();
        }
    }
    else if (t == POWERUP){
        for (unsigned int i = 0 ; i < powerups->size() ; i++){
            if (powerups->at(i)->getInUse())
                powerups->at(i)->update();
        }
    }
    else if (t == SHRAPNEL){
        for (unsigned int i = 0 ; i < shrapnel->size() ; i++){
            if (shrapnel->at(i)->getInUse())
                shrapnel->at(i)->update();
        }
    }
    else if (t == STAR){
        for (unsigned int i = 0 ; i < stars->size() ; i++){
            if (stars->at(i)->getInUse())
                stars->at(i)->update();
        }
    }
}
void ObjectManager::applyForce(const ObjectType t, const double x, const double y, const double mag){
    //once getVector is working, this will get a lot cleaner
    if (t == PARTICLE){
        for (unsigned int i = 0 ; i < particles->size() ; i++){
            if (particles->at(i)->getInUse())
                particles->at(i)->applyForce(x, y, mag);
        }
    }
    else if (t == ENEMY){
        for (unsigned int i = 0 ; i < enemies->size() ; i++){
            if (enemies->at(i)->getInUse())
                enemies->at(i)->applyForce(x, y, mag);
        }
    }
    else if (t == POWERUP){
        for (unsigned int i = 0 ; i < powerups->size() ; i++){
            if (powerups->at(i)->getInUse())
                powerups->at(i)->applyForce(x, y, mag);
        }
    }
    else if (t == SHRAPNEL){
        for (unsigned int i = 0 ; i < shrapnel->size() ; i++){
            if (shrapnel->at(i)->getInUse())
                shrapnel->at(i)->applyForce(x, y, mag);
        }
    }
    else if (t == STAR){
        for (unsigned int i = 0 ; i < stars->size() ; i++){
            if (stars->at(i)->getInUse())
                stars->at(i)->applyForce(x, y, mag);
        }
    }
}

//allocate memory and add the required number of objects
//never need to allocate any more, just reuse the previous ones
void ObjectManager::init(ObjectType t, const unsigned int numObjects){
    if (t == PARTICLE){
        for (unsigned int i = 0 ; i < numObjects ; i++){
            particles->push_back(new Particle());
        }
    }
    else if (t == ENEMY){
        for (unsigned int i = 0 ; i < numObjects ; i++){
            enemies->push_back(new Enemy());
        }
    }
    else if (t == POWERUP){
        for (unsigned int i = 0 ; i < numObjects ; i++){
            powerups->push_back(new Powerup());
        }
    }
    else if (t == SHRAPNEL){
        for (unsigned int i = 0 ; i < numObjects ; i++){
            shrapnel->push_back(new Shrapnel());
        }
    }
    else if (t == STAR){
        for (unsigned int i = 0 ; i < numObjects ; i++){
            stars->push_back(new Star());
        }
    }
}

unsigned int ObjectManager::getNumObjects(ObjectType t){
    /*
    //fix this to return number of active things
    if (t == PARTICLE)
        return particles->size();
    else if (t == ENEMY)
        return enemies->size();
    else if (t == POWERUP)
        return powerups->size();
    else if (t == SHRAPNEL)
        return shrapnel->size();
    else if (t == STAR)
        return stars->size();
    else
    */
        return 0;
}

//return an unused object
//for now, if there aren't any free, it returns NULL
//this can be changed in the future to expand the object pool
GameObject* ObjectManager::getUnused(ObjectType t){
    //once getVector is working, this will get a lot cleaner
    if (t == PARTICLE){
        //special rules for particles (always reuse oldest)
        cur_particle = ++cur_particle % particles->size();
        return particles->at(cur_particle);
    }
    else if (t == ENEMY){
        for (unsigned int i = 0 ; i < enemies->size() ; i++){
            if (!enemies->at(i)->getInUse()){
                return enemies->at(i);
            }
        }
        return NULL;
    }
    else if (t == POWERUP){
        for (unsigned int i = 0 ; i < powerups->size() ; i++){
            if (!powerups->at(i)->getInUse()){
                return powerups->at(i);
            }
        }
        return NULL;
    }
    else if (t == SHRAPNEL){
        for (unsigned int i = 0 ; i < shrapnel->size() ; i++){
            if (!shrapnel->at(i)->getInUse()){
                return shrapnel->at(i);
            }
        }
        return NULL;
    }
    else if (t == STAR){
        for (unsigned int i = 0 ; i < stars->size() ; i++){
            if (!stars->at(i)->getInUse()){
                return stars->at(i);
            }
        }
        return NULL;
    }
    return NULL;
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

void ObjectManager::spawnPowerup(PowerupType t, const double x, const double y, const double x_vel, const double y_vel){

}

void ObjectManager::spawnEnemy(EnemyType t, const double x, const double y, const double x_tar, const double y_tar){

}

void ObjectManager::spawnShrapnel(const double x, const double y, const double x_vel, const double y_vel, const double len, const QColor *clr){

}

//return the proper colour of the particle based on a percentage of its max speed
const QColor* ObjectManager::getParticleCol(const float f){
    return particleCol->at(Util::flr(f * NUM_PARTICLE_COLOURS));
}

Enemy* ObjectManager::getClosestEnemy(const double x, const double y, const double min_dist){
    return NULL;
}
