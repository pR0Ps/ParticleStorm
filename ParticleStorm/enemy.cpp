#include "enemy.h"

using namespace std;

Enemy::Enemy():GameObject(){

}

void Enemy::initialize(double hlth, double spd, double dmg){
	health = hlth;
	speed = spd;
	damage = dmg;
}
