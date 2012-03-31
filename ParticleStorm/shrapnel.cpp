#include "shrapnel.h"
#include "util.h"

const int Shrapnel::MAX_SHRAPNEL_SPEED = 100;
const int Shrapnel::INITIAL_PUSH_MIN = 40;
const int Shrapnel::INITIAL_PUSH_MAX = 140;
const int Shrapnel::MAX_ROTATION_SPD = 600;
const double Shrapnel::INITIAL_TTL = 4;
const int Shrapnel::DECREMENT_SPEED = 3;

Shrapnel::Shrapnel():GameObject(){

}

void Shrapnel::startShrapnel(double x, double y, double x_vel, double y_vel, double len, const QColor *clr){
    this->x = x;
    this->y = y;

    //set velocity
    double tempx = x_vel;
    double tempy = y_vel;
    double tempa = qrand() % 359;
    const double mag = Util::magnitude(x_vel, y_vel);
    if (mag > MAX_SHRAPNEL_SPEED){
        tempx /= mag * MAX_SHRAPNEL_SPEED;
        tempy /= mag * MAX_SHRAPNEL_SPEED;
    }
    this->x_vel = Util::randInt(INITIAL_PUSH_MIN, INITIAL_PUSH_MAX)  * Util::cosd(tempa) + tempx / 2.0f;
    this->y_vel = Util::randInt(INITIAL_PUSH_MIN, INITIAL_PUSH_MAX) * Util::sind(tempa) + tempy / 2.0f;

    this->spin = Util::randInt(-MAX_ROTATION_SPD, MAX_ROTATION_SPD);
    this->len = len;
    this->clr = clr;
    this->ttl = INITIAL_TTL;
    this->inUse = true;
    this->angle =0;
}

void Shrapnel::update(double deltaTime){
    x_vel -= x_vel * DECREMENT_SPEED * deltaTime;
    y_vel -=y_vel *DECREMENT_SPEED*deltaTime;
    x += x_vel * 5 *  deltaTime;
    y += y_vel * 5 * deltaTime;

    angle += spin*deltaTime;
    while (angle > 360) angle -= 360;

    //kill old shrapnel
    if (ttl <= 0) die();
    else ttl -= deltaTime;

}
void Shrapnel::drawFaded() const{

    const double hLen = len/2.0f;

    glPushMatrix();
    glLoadIdentity();
    glTranslated(x, y, 0);
    glRotated(angle, 0, 0, 1);
    Util::drawLine(-hLen, 0, hLen, 0, clr);
    glPopMatrix();
}

