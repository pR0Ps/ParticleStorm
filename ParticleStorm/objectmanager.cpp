#include <QDebug>
#include "math.h"
#include <iostream>
#include "objectmanager.h"

ObjectManager* ObjectManager::instance = NULL;

const int ObjectManager::MAX_PARTICLES = 700;
const int ObjectManager::INIT_ENEMIES = 100;
const int ObjectManager::INIT_POWERUPS = 30;
const int ObjectManager::MAX_SHRAPNEL = 40;
const int ObjectManager::MAX_STARS = 250;
const int ObjectManager::RESIZE_AMT = 20;

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
    initPool(ENEMY, INIT_ENEMIES);
    initPool(POWERUP, INIT_POWERUPS);
    initPool(SHRAPNEL, MAX_SHRAPNEL);
    initPool(STAR, MAX_STARS);

    //set initial index for non-essential entities
    cur_particle = 0;
    cur_shrapnel = 0;

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

    //reset the counters
    cur_particle = 0;
    cur_shrapnel = 0;

    //reset the objects
    deactivateAll(PARTICLE);
    deactivateAll(ENEMY);
    deactivateAll(POWERUP);
    deactivateAll(SHRAPNEL);

    //reset stars
    for (unsigned int i = 0 ; i < stars->size() ; i++) static_cast<Star*>(stars->at(i))->initStar();
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

//convenience function for drawing all the objects
void ObjectManager::drawAll(bool faded){
    if (faded){
        drawFaded(SHRAPNEL);
        drawFaded(POWERUP);
        drawFaded(ENEMY);
        drawFaded(PARTICLE);
        drawFaded(PLAYER);
        //drawFaded(STAR);
    }
    else{
        drawNoFade(STAR);
        drawNoFade(ENEMY);
        drawNoFade(PLAYER);
        //drawNoFade(PARTICLE);
        //drawNoFade(POWERUP);
        //drawNoFade(SHRAPNEL);
    }
}

//apply to all objects
void ObjectManager::drawNoFade(ObjectType t){
    if (t == PLAYER){
        player->drawNoFade();
    }
    else{
        std::vector<GameObject*> temp = getVector(t);
        for (unsigned int i = 0 ; i < temp.size() ; i++){
            if (temp.at(i)->getInUse()){
                temp.at(i)->drawNoFade();
            }
        }
    }
}
void ObjectManager::drawFaded(ObjectType t){
    if (t == PLAYER){
        player->drawFaded();
    }
    else{
        std::vector<GameObject*> temp = getVector(t);
        for (unsigned int i = 0 ; i < temp.size() ; i++){
            if (temp.at(i)->getInUse()){
                temp.at(i)->drawFaded();
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
void ObjectManager::step(ObjectType t, double deltaTime){
    if (t == PLAYER){
        player->step(deltaTime);
    }
    else{
        std::vector<GameObject*> temp = getVector(t);
        for (unsigned int i = 0 ; i < temp.size() ; i++){
            if (temp.at(i)->getInUse()){
                temp.at(i)->step(deltaTime);
            }
        }
    }
}
void ObjectManager::applyForce(const ObjectType t, const double x, const double y, const double mag, const double range, const double dissipation){
    if (t == PLAYER){
        //cannot apply a force to player
        return;
    }
    else{
        std::vector<GameObject*> temp = getVector(t);
        for (unsigned int i = 0 ; i < temp.size() ; i++){
            if (temp.at(i)->getInUse()){
                temp.at(i)->applyForce(x, y, mag, range, dissipation);
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

//extends the pool of objects when more are needed that initiall accounted for
void ObjectManager::extendPool(ObjectType t, unsigned int extra){
    //herp-a-derp
    if (t != ENEMY && t != POWERUP){
        qDebug() << "Tried to resize an object pool that doesn't need be resized";
        exit(1);
    }

    if (t == ENEMY){
        enemies->reserve(enemies->size() + extra);
        for (unsigned int i = 0 ; i < extra ; i++){
            enemies->push_back(new Enemy());
        }
    }
    else if (t == POWERUP){
        powerups->reserve(powerups->size() + extra);
        for (unsigned int i = 0 ; i < extra ; i++){
            powerups->push_back(new Powerup());
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
//if there aren't any free, it expands the object pool
GameObject* ObjectManager::getUnused(ObjectType t){
    if (t == PLAYER){
        //cannot get an unused player
        return NULL;
    }
    else if (t == PARTICLE){
        //always reuse oldest
        cur_particle = ++cur_particle % particles->size();
        return particles->at(cur_particle);
    }
    else if (t == SHRAPNEL){
        //always reuse oldest
        cur_shrapnel = ++cur_shrapnel % shrapnel->size();
        return shrapnel->at(cur_shrapnel);
    }
    else{
        std::vector<GameObject*> temp = getVector(t);
        for (unsigned int i = 0 ; i < temp.size() ; i++){
            if (!temp.at(i)->getInUse()){
                return temp.at(i);
            }
        }
        //resize the array and return the first element of the resized portion
        //NOTE: temp doesn't update with the new vector information, it keeps the old vector
        extendPool(t, RESIZE_AMT);
        return getVector(t).at(temp.size());
    }
}

//return a vector of all things with a range
std::vector<GameObject*>* ObjectManager::getObjectsInRange(ObjectType t, double x, double y, double range){
    std::vector<GameObject*> *ret = new std::vector<GameObject*>;
    std::vector<GameObject*> temp = getVector(t);
    for (unsigned int i = 0 ; i < temp.size() ; i++){
        GameObject *obj = temp.at(i);
        if (obj->getInUse() && Util::distance(x, y, obj->getX(), obj->getY()) - obj->getRadius() < range){
            ret->push_back(obj);
        }
    }
    return ret;
}

//collisions
void ObjectManager::doEnemyParticleCollisions(){

    std::vector<GameObject*> particles = getVector(PARTICLE);
    std::vector<GameObject*> enemies = getVector(ENEMY);

    for(unsigned int i = 0; i < enemies.size(); i++) {
        if(enemies[i]->getInUse()) { //enemies vector stores unused enemies
            for(unsigned int j = 0; j < particles.size(); j++) {
                if(particles[j]->getInUse()) {
                    //check if the enemy is still in use to avoid killing the enemy multiple times
                    if(enemies[i]->getInUse() && Util::distance(particles[j]->getX(), particles[j]->getY(), enemies[i]->getX(), enemies[i]->getY()) < static_cast<Enemy*>(enemies[i])->getRadius()) {
                        enemies[i]->modLife(-static_cast<Particle*>(particles[j])->getSpeedPercent() * Enemy::MAX_DAMAGE,true); //for death by damage
                        particles[j]->die();

                        //draw effects
                    }
                }
            }
        }
    }
}

void ObjectManager::doPlayerEnemyCollisions(){

    //player is immune to damage, don't bother checking
    if (player->isImmune()) return;

    std::vector<GameObject*> enemies = getVector(ENEMY);

    for(unsigned int i = 0; i < enemies.size(); i++) {
        if(enemies[i]->getInUse()) { //enemies vector stores unused enemies
            if(!static_cast<Enemy*>(enemies[i])->isImmune() && playerCollision(player->getX(),player->getY(),player->getXOld(),player->getYOld(),enemies[i]->getX(),enemies[i]->getY(),static_cast<Enemy*>(enemies[i])->getRadius())) {
                player->modLife(-static_cast<Enemy*>(enemies[i])->getDamage(),true);
                enemies[i]->modLife(-Player::RAM_DAMAGE,true);
                //give immunity to the colliding objects
                static_cast<Enemy*>(enemies[i])->setImmune();
                player->setImmune();
                //player is immune now, stop checking

                //draw effects

                return;
            }
        }
    }

}

void ObjectManager::doPlayerPowerupCollisions(){

    std::vector<GameObject*> powerups = getVector(POWERUP);

    for(unsigned int i = 0; i < powerups.size(); i++) {
        if(powerups[i]->getInUse()) { //powerups vector stores unused enemies
            if(playerCollision(player->getX(),player->getY(),player->getXOld(),player->getYOld(), powerups[i]->getX(),powerups[i]->getY(),static_cast<Powerup*>(powerups[i])->getRadius())) {
                //do player powerups collision
                Powerup* pow = static_cast<Powerup*>(powerups[i]);
                //mod the player
                if (pow->getType() == ObjectManager::HEALTH){
                    player->modLife(pow->getValue() * Powerup::VALUE_RATIO_HEALTH);
                }
                else if (pow->getType() == ObjectManager::MANA){
                     player->modMana(pow->getValue() * Powerup::VALUE_RATIO_MANA);
                }

                //draw effects (how? in what class?)

                //kill the powerup
                pow->die();
            }
        }
    }
}

bool ObjectManager::playerCollision(double px1, double py1, double px2, double py2, double ex, double ey, double radius) {

    double dotprod = (ex-px1)*(px2-px1)+(ey-py1)*(py2-py1);
    double mag = Util::magnitude(px2-px1,py2-py1);
    double closestX, closestY;
    if (px1!=px2 && py1!=py2) {
        if (dotprod/mag < 0){
            closestX = px1;
            closestY = py1;
        }
        else if (dotprod/mag > 1){
            closestX = px2;
            closestY = py2;
        }
        else {
            closestX = px1 + (dotprod/mag)*(px2-px1);
            closestY = py1 + (dotprod/mag)*(py2-py1);
        }
        //qDebug() << "New:" << px1 << py1 << "Old:" << px2 << py2 << "Slp:" << (px2-px1)/(py2-py1) << "Clse(x y):" << closestX << closestY;
        return Util::magnitude(ex-closestX,ey-closestY) < (radius+player->getRadius());
    }
    else
        return Util::magnitude(ex-px1,ey-py2) < (radius+player->getRadius());
}

//spawning
void ObjectManager::spawnParticle(const double x, const double y, const double x_vel, const double y_vel){
    static_cast<Particle*>(getUnused(PARTICLE))->startParticle(x, y, x_vel, y_vel);
}

void ObjectManager::spawnParticle(const double x, const double y){
    const unsigned int angle = qrand() % 359;
    const int mag = qrand() % 120 / 5.0;
    spawnParticle(x, y, mag * Util::cosd(angle), mag * Util::sind(angle));
}

void ObjectManager::spawnPowerup (PowerupType t, double x, double y, double x_vel, double y_vel, int value){
    static_cast<Powerup*>(getUnused(POWERUP))->startPowerup(t, x, y, x_vel, y_vel, value);
}

//spawn an enemy
void ObjectManager::spawnEnemy(EnemyType t, const double x, const double y, const double x_tar, const double y_tar){
    static_cast<Enemy*>(getUnused(ENEMY))->startEnemy(t, x, y, x_tar, y_tar);
}
void ObjectManager::spawnEnemy(EnemyType t, double x, double y){
    spawnEnemy(t, x, y, getPlayer()->getX(), getPlayer()->getY());
}
void ObjectManager::spawnEnemy(EnemyType t, Util::Point2D p){
    spawnEnemy(t, p.x, p.y, getPlayer()->getX(), getPlayer()->getY());
}
void ObjectManager::spawnEnemy(Util::Point2D p){
    spawnEnemy((EnemyType)Util::randInt(GRUNT, SHOOTER), p.x, p.y, getPlayer()->getX(), getPlayer()->getY());
}

void ObjectManager::spawnShrapnel(const double x, const double y, const double x_vel, const double y_vel, const int num, const double len, const Util::Color *clr){
    for (int i = 0 ; i < num ; i++){
        static_cast<Shrapnel*>(getUnused(SHRAPNEL))->startShrapnel(x, y, x_vel, y_vel, len, clr);
    }
}

Enemy* ObjectManager::getClosestEnemy(double x, double y, double minDistance,
                                      double maxDistance) {
    Enemy* closestEnemy = NULL;
    int closestDistance = INT_MAX;

    for (unsigned int i = 0; i < enemies->size(); i++) {
        // The cast is needed because the enemies vector is declared as a
        // vector of GameObject pointers.
        Enemy* currentEnemy = dynamic_cast<Enemy*> (enemies->at(i));
        int distance = Util::distance(x, y, currentEnemy->getX(),
                                      currentEnemy->getY());

        // Perform bounds checking on the distance to the currently selected
        // enemy and determine if this is the new closest enemy. Also, don't
        // consider the enemy if it's not in use.
        if (distance >= minDistance && distance <= maxDistance &&
                distance < closestDistance && currentEnemy->getInUse()) {
            closestEnemy = currentEnemy;
            closestDistance = distance;
        }
    }

    return closestEnemy;
}

//mod player
void ObjectManager::modPlayerLife(int amt, bool rel) {
    player->modLife(amt, rel);
}
void ObjectManager::modPlayerMana(int amt, bool rel){
    player->modMana(amt, rel);
}
void ObjectManager::modPlayerScore(int amt, bool rel) {
    player->modScore(amt, rel);
} 
