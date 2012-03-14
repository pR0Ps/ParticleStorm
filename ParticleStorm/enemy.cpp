#include "enemy.h"

#include <string>

using namespace std;

//canvas is 1024x768
Enemy::Enemy():GameObject(){

}

//y u no enemytype?
void Enemy::startEnemy(int t, double x, double y, double x_tar, double y_tar){
        inUse = true;
	type = t;

	//give starting stats depending on type
    if(type == ObjectManager::GRUNT){ 			//grunt
        maxLife = life = 100;
        speed = 50;
        damage = 10;
    }
    else if(type == ObjectManager::SHOOTER){ 	//shooter
        maxLife = life = 75;
        speed = 50;
        damage = 10;
    }
    else if(type == ObjectManager::HEALER){ 		//healer
        maxLife = life = 100;
        speed = 50;
        damage = 10;
    }
    else if(type == ObjectManager::TANK){ 		//tank
        maxLife = life = 300;
        speed = 25;
        damage = 10;
    }
    else if(type == ObjectManager::SPRINTER){	//sprinter
        maxLife = life = 100;
        speed = 100;
        damage = 10;
    }
}

void Enemy::move(double dx, double dy){
    x += dx;
    y += dy;
}

void Enemy::update(){
    if(type == ObjectManager::GRUNT || type == ObjectManager::TANK || type == ObjectManager::SPRINTER){
        //get player position
        //double xpos = user.getX();
        //double ypos = user.getY();
        //find position of enemy relative to player
        //double xrel = xpos - x;
        //double yrel = ypos - y;
        //take speed into account (updates 100 times per second)
        //double xdist = (xrel/100)*speed;
        //double ydist = (yrel/100)*speed;
        //move the enemy towards player
        //move(xdist,ydist);
    }
    else if(type == ObjectManager::SHOOTER){
        //shooter
    }
    else{
        //healer
    }
}

void Enemy::draw() const{
	//needs openGL
}
