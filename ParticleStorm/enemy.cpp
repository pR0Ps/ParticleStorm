#include "enemy.h"
#include "gameengine.h"
#include <QDebug>

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

    //calculating the x and y distance from enemy spawn and current player position
    double x_dist = x_tar - x;
    double y_dist = y_tar - y;

    //calculate angle
    double theta = Util::atand(y_dist,x_dist);

    //calculating the x and y values to make a unit vector pointing at player
    x_vel = cos(theta);
    y_vel = sin(theta);

    //give starting stats depending on type
    if(type == ObjectManager::GRUNT){ 			//grunt
        maxLife = life = 100;
        speed = 1;
        damage = 10;
    }
    else if(type == ObjectManager::SHOOTER){ 	//shooter
        maxLife = life = 75;
        speed = 1;
        damage = 10;
    }
    else if(type == ObjectManager::HEALER){ 		//healer
        maxLife = life = 100;
        speed = 1;
        damage = 10;
    }
    else if(type == ObjectManager::TANK){ 		//tank
        maxLife = life = 300;
        speed = 0.5;
        damage = 10;
    }
    else if(type == ObjectManager::SPRINTER){	//sprinter
        maxLife = life = 100;
        speed = 2;
        damage = 10;
    }

    clr = ResourceManager::getInstance()->getColour(ResourceManager::RED);
}

void Enemy::update(double deltaTime){
    if(type == ObjectManager::GRUNT || type == ObjectManager::TANK || type == ObjectManager::SPRINTER){

        x += x_vel * speed * deltaTime;
        y += y_vel * speed * deltaTime;

    }
    else if(type == ObjectManager::SHOOTER){
        //shooter
    }
    else{
        //healer
    }
}

void Enemy::draw() const{
    Util::drawBox(x-100,y-100,x+100,y+100,false,clr);
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
}
