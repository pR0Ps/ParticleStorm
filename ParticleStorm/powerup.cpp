#include "powerup.h"
#include "gameengine.h"

const float Powerup::AIR_RESIST = 1;
const int Powerup::MAX_ROTATION_SPD = 900;
const int Powerup::MIN_ROTATION_SPD = 540;
const double Powerup::INITIAL_TTL = 10;
const int Powerup::NUM_LIGHTNING = 5;
const double Powerup::COLLECT_RING_SIZE = 0.8;
const double Powerup::VALUE_RATIO_HEALTH = 0.5;
const double Powerup::VALUE_RATIO_MANA = 0.5;

Powerup::Powerup():GameObject(){
    //do nothing
}

void Powerup::startPowerup(int type, double x, double y, double x_vel, double y_vel, int value){
    this->type = type;
    this->value = value;
    this->x = x;
    this->y = y;
    this->x_vel = x_vel;
    this->y_vel = y_vel;
    this->spin = Util::randInt(MIN_ROTATION_SPD, MAX_ROTATION_SPD) * ((qrand() % 2) * 2 - 1);
    this->angle = 0;
    this->ttl = INITIAL_TTL;
    this->radius = 25;
    this->inUse = true;

    //set colour based on type
    if (type == ObjectManager::HEALTH)
        this->clr = ResourceManager::getInstance()->getColour(ResourceManager::LIGHTRED);
    else if (type == ObjectManager::MANA)
        this->clr = ResourceManager::getInstance()->getColour(ResourceManager::LIGHTBLUE);
    else{
        qDebug() << type;
        this->clr = ResourceManager::getInstance()->getColour(ResourceManager::WHITE);
    }
}

void Powerup::drawFaded() const{
    glPushMatrix();
    glLoadIdentity();
    glTranslated(x, y, 0);
    glRotated(angle, 0, 0, 1);
    Util::drawRoundShape(0, 0, radius, 3, false, clr);
    glPopMatrix();
}

void Powerup::step(double deltaTime){
    //adjust velocity
    x_vel -= AIR_RESIST * x_vel * deltaTime;
    y_vel -= AIR_RESIST * y_vel * deltaTime;

    //spin the particle
    angle += deltaTime * spin;
    if (angle > 360) angle = angle - 360;

    //change position
    x += x_vel * deltaTime;
    y += y_vel * deltaTime;

    //kill old powerups
    if (ttl <= 0)
        die();
    else
        this->ttl -= deltaTime;
}
