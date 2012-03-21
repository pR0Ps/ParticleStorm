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

    //calculating the x and y distance from enemy spawn and current player position
    double x_dist = x_tar - x;
    double y_dist = y_tar - y;

    //calculate angle
    double theta = atan(y_dist/x_dist);

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
}

void Enemy::update(){
    if(type == ObjectManager::GRUNT || type == ObjectManager::TANK || type == ObjectManager::SPRINTER){
        dt = (1/(double)GameEngine::MAX_FPS);

        x += x_vel * speed * dt;
        y += y_vel * speed * dt;
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

void Enemy::die(){
    inUse = false;
}
