#include "enemy.h"

#include <string>

using namespace std;

//canvas is 1024x768
Enemy::Enemy(string t):GameObject(){
	active = true;
	type = t;

	//give starting stats depending on type
    if(type == 'grunt'){ 			//grunt
        health = 100;
        speed = 50;
        damage = 10;
    }
    else if(type == 'shooter'){ 	//shooter
        health = 75;
        speed = 50;
        damage = 10;
    }
    else if(type == 'healer'){ 		//healer
        health = 100;
        speed = 50;
        damage = 10;
    }
    else if(type == 'tank'){ 		//tank
        health = 300;
        speed = 25;
        damage = 10;
    }
    else if(type == 'sprinter'){	//sprinter
        health = 100;
        speed = 100;
        damage = 10;
    }

    //determine start position at the edge of the screen
    int start = qrand()%4;
    if(start == 0){
        x = 0;
        y = qrand()%768;
    }
    else if(start == 1){
        x = 1024;
        y = qrand()%768;
    }
    else if(start == 2){
        x = qrand()%1024;
        y = 0;
    }
    else if(start == 3){
        x = qrand()%1024;
        y = 768;
    }
}

void Enemy::reset(string t){
	active = true;
	type = t;

	//give starting stats depending on type
    if(type == 'grunt'){ 			//grunt
        health = 100;
        speed = 50;
        damage = 10;
    }
    else if(type == 'shooter'){ 	//shooter
        health = 75;
        speed = 50;
        damage = 10;
    }
    else if(type == 'healer'){ 		//healer
        health = 100;
        speed = 50;
        damage = 10;
    }
    else if(type == 'tank'){ 		//tank
        health = 300;
        speed = 25;
        damage = 10;
    }
    else if(type == 'sprinter'){	//sprinter
        health = 100;
        speed = 100;
        damage = 10;
    }

    //determine start position at the edge of the screen
    int start = qrand()%4;
    if(start == 0){
        x = 0;
        y = qrand()%768;
    }
    else if(start == 1){
        x = 1024;
        y = qrand()%768;
    }
    else if(start == 2){
        x = qrand()%1024;
        y = 0;
    }
    else if(start == 3){
        x = qrand()%1024;
        y = 768;
    }
}
}

void Enemy::move(double dx, double dy){
    x += dx;
    y += dy;
}

//every time an enemy gets hit by a particle, or healed by a healer
//Q: damage will be calculated in particle class?
void Enemy::changeHealth(double d){
    health += d;
    if(health >= MAXHLTH){
        health = MAXHLTH;
    }
    else if(health <= 0){
        active = false;
    }
}

void Enemy::update(Player user){
	if(type == 'grunt' || type == 'tank' || type == 'sprinter'){
		//get player position
		double xpos = user.getX();
		double ypos = user.getY();
		//find position of enemy relative to player
		double xrel = xpos - x;
		double yrel = ypos - y;
		//take speed into account (updates 100 times per second)
		double xdist = (xrel/100)*speed;
		double ydist = (yrel/100)*speed;
		//move the enemy towards player
		move(xdist,ydist);
	}
	else if(type == 'shooter'){
		//shooter
	}
	else{
		//healer
	}
}

void Enemy::draw(){
	//needs openGL
}
