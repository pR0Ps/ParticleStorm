#include "enemy.h"
#include "gameengine.h"
#include <QDebug>
#include <cmath>

const int Enemy::MAX_DAMAGE;


Enemy::Enemy():GameObject(){
}

//y u no enemytype?
void Enemy::startEnemy(int t, double x, double y, double x_tar, double y_tar){
    inUse = true;
    type = t;
    this->x=x;
    this->y=y;
    this->x_tar = x_tar;
    this->y_tar = y_tar;
    this->collisionBufferTime = 0;

    //give starting stats depending on type
    if(type == ObjectManager::GRUNT){
        maxLife = life = 100;
        speed = 100;
        damage = 10;
        numShrapnel = 4;
        shrapnelLen = 40;
        radius = 20;

        //calculating the x and y distance from enemy spawn and current player position
        double x_dist = x_tar - x;
        double y_dist = y_tar - y;

        //calculate angle in radians
        double theta = atan(y_dist/x_dist);

        //calculating the x and y values to make a unit vector pointing at player
        x_vel = cos(theta);
        y_vel = sin(theta);
        if(x_tar < x && y_tar < y){
            x_vel = -x_vel;
            y_vel = -y_vel;
        }
        else if(x_tar < x && y_tar > y)
            x_vel = -x_vel;
        else if(x_tar > x && y_tar < y)
            y_vel = -y_vel;
    }
    else if(type == ObjectManager::HEALER){
        maxLife = life = 100;
        speed = 100;
        damage = 10;
        numShrapnel = 6;
        shrapnelLen = 10;
        radius = 15;
    }
    else if(type == ObjectManager::TANK){
        maxLife = life = 300;
        speed = 50;
        damage = 10;
        numShrapnel = 8;
        shrapnelLen = 15;
        radius = 30;

        //calculating the x and y distance from enemy spawn and current player position
        double x_dist = x_tar - x;
        double y_dist = y_tar - y;

        //calculate angle in radians
        double theta = atan(y_dist/x_dist);

        //calculating the x and y values to make a unit vector pointing at player
        x_vel = cos(theta);
        y_vel = sin(theta);
        if(x_tar < x && y_tar < y){
            x_vel = -x_vel;
            y_vel = -y_vel;
        }
        else if(x_tar < x && y_tar > y)
            x_vel = -x_vel;
        else if(x_tar > x && y_tar < y)
            y_vel = -y_vel;
    }
    else if(type == ObjectManager::SPRINTER){
        maxLife = life = 100;
        speed = 200;
        damage = 10;
        numShrapnel = 4;
        shrapnelLen = 30;
        radius = 15;

        //calculating the x and y distance from enemy spawn and current player position
        double x_dist = x_tar - x;
        double y_dist = y_tar - y;

        //calculate angle in radians
        double theta = atan(y_dist/x_dist);

        //calculating the x and y values to make a unit vector pointing at player
        x_vel = cos(theta);
        y_vel = sin(theta);
        if(x_tar < x && y_tar < y){
            x_vel = -x_vel;
            y_vel = -y_vel;
        }
        else if(x_tar < x && y_tar > y)
            x_vel = -x_vel;
        else if(x_tar > x && y_tar < y)
            y_vel = -y_vel;
    }
    else if(type == ObjectManager::SHOOTER){
        maxLife = life = 75;
        speed = 100;
        damage = 10;
        numShrapnel = 4;
        shrapnelLen = 25;
        radius = 12;
    }
    else if (type == ObjectManager::BULLET){
        maxLife = life = 10;
        speed = 1000;
        damage = 10;
        numShrapnel = 0;
        shrapnelLen = 0;
        radius = 7;
    }
    else{
        qDebug() << "bad enemy type";
        exit(1);
    }
    clr = ResourceManager::getInstance()->getColour(ResourceManager::RED);
}

void Enemy::update(double deltaTime){

    if(abs(x - x_tar) == 2048 || abs(y - y_tar) == 1536){
        die();
    }

    if(type == ObjectManager::GRUNT || type == ObjectManager::TANK || type == ObjectManager::SPRINTER){

        x += x_vel * speed * deltaTime;
        y += y_vel * speed * deltaTime;

    }
    else if(type == ObjectManager::SHOOTER){

        //move to either top or bottom of screen, depending on which is closer
        if(x < ((GameEngine::MAX_X)/2) && x > 10){
            x -= speed * deltaTime;
        }
        else if(x >= ((GameEngine::MAX_X)/2) && x < 758){
            x += speed * deltaTime;
        }
        else if(x > 758){
            x -= speed * deltaTime;
        }
        else if(x < 10){
            x += speed * deltaTime;
        }

        //move to either left or right of screen, depending on which is closer
        if(y < ((GameEngine::MAX_Y)/2) && y > 10){
            y -= speed * deltaTime;
        }
        else if(y >= ((GameEngine::MAX_Y)/2) && y < 1014){
            y += speed * deltaTime;
        }
        else if(y > 1014){
            y -= speed * deltaTime;
        }
        else if(y < 10){
            y += speed * deltaTime;
        }

    }
    else{
        //healer
    }

    if(collisionBufferTime > 0)
        collisionBufferTime -= deltaTime;

}

void Enemy::draw() const{
    Util::drawOctagon(x, y, radius * 2, false, clr);
    Util::drawBox(x-radius,y-radius,x+radius,y+radius,false,clr);
}

//pan the enemy (and their target)
void Enemy::pan(double x, double y){
    this->x += x;
    this->y += y;
    this->x_tar += x_tar;
    this->y_tar += y_tar;
}

void Enemy::die(){
    inUse = false;

    //enemy died, create some shrapnel with it's position and velocity
    ObjectManager::getInstance()->spawnShrapnel(x, y, x_vel, y_vel, numShrapnel, shrapnelLen, clr);
}
