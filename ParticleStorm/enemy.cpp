#include "enemy.h"
#include "gameengine.h"
#include <QDebug>
#include <cmath>

const int Enemy::MAX_DAMAGE;
const double Enemy::MAX_COLLISION_BUFFER_TIME;
const int Enemy::MIN_ROTATION_SPD;
const int Enemy::MAX_ROTATION_SPD;

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
    this->shotCounter = 0;
    this->healCounter = 0;
    this->angle = 0;
    this->spin = Util::randInt(MIN_ROTATION_SPD, MAX_ROTATION_SPD) * ((qrand() % 2) * 2 - 1);
    currentEnemy = NULL;

    //give starting stats depending on type
    if(type == ObjectManager::GRUNT){
        maxLife = life = 100;
        speed = 150;
        damage = 10;
        numShrapnel = 4;
        shrapnelLen = 40;
        radius = 20;
        clr = ResourceManager::getInstance()->getColour(ResourceManager::BLUE);

        findDirection(x, y, x_tar, y_tar);
    }
    else if(type == ObjectManager::HEALER){
        maxLife = life = 100;
        speed = 150;
        damage = 10;
        numShrapnel = 6;
        shrapnelLen = 10;
        radius = 15;
        healStart = true;
        clr = ResourceManager::getInstance()->getColour(ResourceManager::GREEN);
        const Enemy* enemy;
    }
    else if(type == ObjectManager::TANK){
        maxLife = life = 300;
        speed = 75;
        damage = 10;
        numShrapnel = 8;
        shrapnelLen = 15;
        radius = 30;
        clr = ResourceManager::getInstance()->getColour(ResourceManager::GREEN);

        findDirection(x, y, x_tar, y_tar);
    }
    else if(type == ObjectManager::SPRINTER){
        maxLife = life = 100;
        speed = 300;
        damage = 10;
        numShrapnel = 4;
        shrapnelLen = 30;
        radius = 15;
        clr = ResourceManager::getInstance()->getColour(ResourceManager::ORANGE);

        findDirection(x, y, x_tar, y_tar);
    }
    else if(type == ObjectManager::SHOOTER){
        maxLife = life = 75;
        speed = 150;
        damage = 10;
        numShrapnel = 4;
        shrapnelLen = 25;
        radius = 12;
        clr = ResourceManager::getInstance()->getColour(ResourceManager::YELLOW);
    }
    else if (type == ObjectManager::BULLET){
        maxLife = life = 10;
        speed = 1000;
        damage = 10;
        numShrapnel = 0;
        shrapnelLen = 0;
        radius = 7;

        findDirection(x, y, x_tar, y_tar);
        clr = ResourceManager::getInstance()->getColour(ResourceManager::RED);
    }
    else{
        qDebug() << "bad enemy type";
        exit(1);
    }
}

void Enemy::update(double deltaTime){

    if(this->getX() > 1024 || this->getX() < 0 || this->getY() > 768 || this->getY() < 0){
        if(this->getX() > 1024 || this->getY() > 768)
            Util::drawRoundShape(Util::min(1024, this->getX()), Util::min(768, this->getY()), 10, 3, false);
        else if(this->getX() > 1024 || this->getY() < 0)
            Util::drawRoundShape(Util::min(1024, this->getX()), Util::max(0, this->getY()), 10, 3, false);
        else if(this->getX() < 0 || this->getY() > 768)
            Util::drawRoundShape(Util::max(0, this->getX()), Util::min(768, this->getY()), 10, 3, false);
        else if(this->getX() < 0 || this->getY() < 0)
            Util::drawRoundShape(Util::max(0, this->getX()), Util::max(0, this->getY()), 10, 3, false);
    }

    const Player* player = ObjectManager::getInstance()->getPlayer();

    if(type == ObjectManager::GRUNT || type == ObjectManager::TANK || type == ObjectManager::SPRINTER){

        if(x >= (GameEngine::MAX_X) || x <= 0 || y >= (GameEngine::MAX_Y) || y <= 0){
            findDirection(x, y, player->getX(), player->getY());
        }
        x += x_vel * speed * deltaTime;
        y += y_vel * speed * deltaTime;

    }
    else if(type == ObjectManager::SHOOTER){

        shotCounter++;

        //if shotCount reaches its limit, fire a shot
        if(shotCounter == 120){
            shotCounter = 0;
            ObjectManager::getInstance()->spawnEnemy(ObjectManager::BULLET, x, y, player->getX(), player->getY());
        }

        if(x < 40){
            x += 2 * speed * deltaTime;
        }
        else if(x > 984){
            x -= 2 * speed * deltaTime;
        }

        if(y < 40){
            y += 2 * speed * deltaTime;
        }
        else if(y > 728){
            y -= 2 * speed * deltaTime;
        }

        if(Util::distance(x, y, player->getX(), player->getY()) < 500){
            findDirection(x, y, player->getX(), player->getY());

            x -= x_vel * speed * deltaTime;
            y -= y_vel * speed * deltaTime;
        }
    }
    else if(type == ObjectManager::HEALER){

        healCounter++;
        if(healCounter == 360 || healStart == true){
            healStart = false;
            healCounter = 0;
            currentEnemy = ObjectManager::getInstance()->getClosestEnemy(x, y);
        }

        if(currentEnemy->inUse == true && Util::distance(x, y, currentEnemy->getX(), currentEnemy->getY()) > 50){
            findDirection(x, y, currentEnemy->getX(), currentEnemy->getY());
            x += x_vel * speed * deltaTime;
            y += y_vel * speed * deltaTime;
        }
        else if(currentEnemy->inUse == true && Util::distance(x, y, currentEnemy->getX(), currentEnemy->getY()) <= 50){
            currentEnemy->modLife(10*deltaTime, true);
        }
        else{
            currentEnemy = ObjectManager::getInstance()->getClosestEnemy(x, y);
        }

    }
    else if(type == ObjectManager::BULLET){
        x += x_vel * speed * deltaTime;
        y += y_vel * speed * deltaTime;

        if(x >= (GameEngine::MAX_X)*2 || y >= (GameEngine::MAX_Y)*2){
            die();
        }
    }

    //spin the enemies
    angle += deltaTime * spin;
    if (angle > 360) angle = angle - 360;

    //decrease collision buffer time (should be in player)
    if(collisionBufferTime > 0)
        collisionBufferTime -= deltaTime;

}

void Enemy::draw() const{
    //set up a new matrix for drawing
    glPushMatrix();
    glLoadIdentity();
    glTranslated(x, y, 0);
    glRotated(angle, 0, 0, 1);

    //draw the different objects
    if (type == ObjectManager::GRUNT){
       Util::drawBox(-radius, -radius, radius, radius, false, clr);
       glRotated(45, 0, 0, 1);
       const double temp = sqrt(radius);
       Util::drawBox(-radius + temp, -radius + temp, radius - temp, radius - temp, false, clr);
    }
    else if (type == ObjectManager::HEALER){
        Util::drawRoundShape(0, 0, radius * 2, 5, false, clr);
    }
    else if (type == ObjectManager::TANK){
        Util::drawRoundShape(0, 0, radius * 2, 6, false, clr);
        glRotated(30, 0, 0, 1);
        Util::drawRoundShape(0, 0, radius, 6, false, clr);
    }
    else if (type == ObjectManager::SPRINTER){
        Util::drawBox(-radius, -radius, radius, radius, false, clr);
    }
    else if (type == ObjectManager::SHOOTER){
        Util::drawBox(-radius, -radius, radius, radius, false, clr);
    }
    else{//bullet
        Util::drawRoundShape(0, 0, radius * 2, 8, false, clr);
    }
    glPopMatrix();
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

void Enemy::findDirection(double x2, double y2, double x_tar2, double y_tar2){

    //calculating the x and y distance from enemy spawn and current player position
    double x_dist = x_tar2 - x2;
    double y_dist = y_tar2 - y2;

    //calculate angle in radians
    double theta = atan(y_dist/x_dist);

    //calculating the x and y values to make a unit vector pointing at player
    x_vel = cos(theta);
    y_vel = sin(theta);
    if(x_tar2 < x2 && y_tar2 < y2){
        x_vel = -x_vel;
        y_vel = -y_vel;
    }
    else if(x_tar2 < x2 && y_tar2 > y2)
        x_vel = -x_vel;
    else if(x_tar2 > x2 && y_tar2 < y2)
        y_vel = -y_vel;
}
