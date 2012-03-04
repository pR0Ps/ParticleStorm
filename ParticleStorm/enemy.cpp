#include "enemy.h"

using namespace std;

Enemy::Enemy():GameObject(){

}

void Enemy::draw() const{

}

void Enemy::update(){

}

void Enemy::applyForce(double x, double y, double mag){

}

void Enemy::initialize(double hlth, double spd, double dmg){
	health = hlth;
	speed = spd;
	damage = dmg;
}
