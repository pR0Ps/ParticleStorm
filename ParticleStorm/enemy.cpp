#include "enemy.h"
#include "gameengine.h"
#include <QDebug>
#include <cmath>

const int Enemy::MAX_DAMAGE;
const double Enemy::MAX_COLLISION_BUFFER_TIME;
const int Enemy::MIN_ROTATION_SPD;
const int Enemy::MAX_ROTATION_SPD;
const int Enemy::OOB_LIMIT;

Enemy::Enemy():GameObject(){
}

//y u no enemytype?
void Enemy::startEnemy(int t, double x, double y, double x_tar, double y_tar){
    this->type = t;
    this->x=x;
    this->y=y;
    this->x_tar = x_tar;
    this->y_tar = y_tar;

    collisionBufferTime = 0;
    angle = 0;
    spin = Util::randInt(MIN_ROTATION_SPD, MAX_ROTATION_SPD) * ((qrand() % 2) * 2 - 1);
    currentEnemy = NULL;
    inUse = true;

    //give starting stats depending on type
    if(type == ObjectManager::GRUNT){
        maxLife = life = 100;
        speed = 150;
        damage = 50;
        points = 10;
        numShrapnel = 4;
        shrapnelLen = 40;
        radius = 20;
        clr = ResourceManager::getInstance()->getColour(ResourceManager::BLUE);

        findDirection(x, y, x_tar, y_tar);
    }
    else if(type == ObjectManager::HEALER){
        maxLife = life = 100;
        speed = 150;
        damage = 20;
        points = 12;
        numShrapnel = 6;
        shrapnelLen = 10;
        radius = 15;
        timerActive = true;
        currTimer = maxTimer = 5; //5 seconds
        clr = ResourceManager::getInstance()->getColour(ResourceManager::GREEN);
    }
    else if(type == ObjectManager::TANK){
        maxLife = life = 600;
        speed = 75;
        damage = 75;
        points = 23;
        numShrapnel = 8;
        shrapnelLen = 15;
        radius = 30;
        clr = ResourceManager::getInstance()->getColour(ResourceManager::GREEN);

        findDirection(x, y, x_tar, y_tar);
    }
    else if(type == ObjectManager::SPRINTER){
        maxLife = life = 100;
        speed = 800;
        damage = 50;
        points = 15;
        numShrapnel = 4;
        shrapnelLen = 30;
        radius = 15;
        timerActive = false;
        currTimer = maxTimer = (3 + (qrand()%2))/(3 + (qrand()%2));
        moving = false;
        clr = ResourceManager::getInstance()->getColour(ResourceManager::ORANGE);

        findDirection(x, y, x_tar, y_tar);
    }
    else if(type == ObjectManager::SHOOTER){
        maxLife = life = 75;
        speed = 150;
        damage = 20;
        points = 19;
        numShrapnel = 4;
        shrapnelLen = 25;
        radius = 12;
        currTimer = maxTimer = 2;
        timerActive = true;
        clr = ResourceManager::getInstance()->getColour(ResourceManager::YELLOW);
    }
    else if (type == ObjectManager::BULLET){
        maxLife = life = 10;
        speed = 1000;
        damage = 50;
        points = 0;
        numShrapnel = 0;
        shrapnelLen = 0;
        radius = 7;
        clr = ResourceManager::getInstance()->getColour(ResourceManager::RED);

        findDirection(x, y, x_tar, y_tar);

    }
    else{
        qDebug() << "bad enemy type";
        exit(1);
    }
}

void Enemy::update(double deltaTime){

    const Player* player = ObjectManager::getInstance()->getPlayer();

    //update the action timer
    if (timerActive && currTimer > 0)
        currTimer -= deltaTime;

    if(Util::coordInRect(x, y, 0, 0, GameEngine::MAX_X, GameEngine::MAX_Y) == false && type != ObjectManager::BULLET){
        findDirection(x, y, player->getX(), player->getY());
        x += x_vel * speed * deltaTime;
        y += y_vel * speed * deltaTime;
    }

    else{
        if(type == ObjectManager::GRUNT){

            if(x >= (GameEngine::MAX_X) || x <= 0 || y >= (GameEngine::MAX_Y) || y <= 0){
                findDirection(x, y, player->getX(), player->getY());
            }
            x += x_vel * speed * deltaTime;
            y += y_vel * speed * deltaTime;

        }
        else if (type == ObjectManager::TANK){
            //to be added: apply force to particles
            if(x >= (GameEngine::MAX_X) || x <= 0 || y >= (GameEngine::MAX_Y) || y <= 0){
                findDirection(x, y, player->getX(), player->getY());
            }
            x += x_vel * speed * deltaTime;
            y += y_vel * speed * deltaTime;
        }
        else if (type == ObjectManager::SPRINTER){
            if(currTimer == maxTimer && !timerActive && !moving){
                findDirection(x, y, player->getX(), player->getY());
                playerX = player->getX();
                playerY = player->getY();
                timerActive = true;
            }
            else if(currTimer <= 0 && timerActive && !moving){
                currTimer = maxTimer;
                timerActive = false;
                moving = true;
            }
            else if(currTimer <= 0 && timerActive && moving){
                currTimer = maxTimer;
                timerActive = false;
                moving = false;
            }
            else if(moving){
                timerActive = true;
                x += x_vel * speed * deltaTime;
                y += y_vel * speed * deltaTime;
            }
        }
        else if(type == ObjectManager::SHOOTER){

            //if the timer permits, fire a shot
            if(currTimer <= 0){
                currTimer = maxTimer;
                ObjectManager::getInstance()->spawnEnemy(ObjectManager::BULLET, x, y, player->getX(), player->getY());
            }

            if(x < 40)
                x += 2 * speed * deltaTime;
            else if(x > 984)
                x -= 2 * speed * deltaTime;
            if(y < 40)
                y += 2 * speed * deltaTime;
            else if(y > 728)
                y -= 2 * speed * deltaTime;

            if(Util::distance(x, y, player->getX(), player->getY()) < 500){
                findDirection(x, y, player->getX(), player->getY());

                x -= x_vel * speed * deltaTime;
                y -= y_vel * speed * deltaTime;
            }
        }
        else if(type == ObjectManager::HEALER){

            //go to the next enemy when the timer runs out (or when not doing anything else)
            if((currTimer <= 0 && timerActive) || currentEnemy == NULL){
                currTimer = maxTimer;
                timerActive = false; //don't start running the timer until an enemy is getting healed
                currentEnemy = ObjectManager::getInstance()->getClosestEnemy(x, y);
            }

            //moving towards a target
            if (currentEnemy != NULL){
                if(currentEnemy->inUse && Util::distance(x, y, currentEnemy->getX(), currentEnemy->getY()) > 50){
                    findDirection(x, y, currentEnemy->getX(), currentEnemy->getY());
                    x += x_vel * speed * deltaTime;
                    y += y_vel * speed * deltaTime;
                }
                else if(currentEnemy->inUse && Util::distance(x, y, currentEnemy->getX(), currentEnemy->getY()) <= 50){
                    currentEnemy->modLife(10*deltaTime, true);
                    timerActive = true; //start the timer
                }
                else
                    currentEnemy = ObjectManager::getInstance()->getClosestEnemy(x, y);
            }
            else{
                findDirection(x, y, player->getX(), player->getY());
                x += x_vel * speed * deltaTime;
                y += y_vel * speed * deltaTime;
            }
        }
        else if(type == ObjectManager::BULLET){
            x += x_vel * speed * deltaTime;
            y += y_vel * speed * deltaTime;

            if (!Util::coordInRect(x, y, -OOB_LIMIT, -OOB_LIMIT, GameEngine::MAX_X + OOB_LIMIT, GameEngine::MAX_Y + OOB_LIMIT)){
                die();
            }
        }
    }
    //spin the enemies
    angle += deltaTime * spin;
    if (angle > 360) angle = angle - 360;

    //decrease collision buffer time (should be in player)
    if(collisionBufferTime > 0)
        collisionBufferTime -= deltaTime;
}

void Enemy::drawNoFade() const{

    //draw indicators for enemies that are off-screen
    if (!Util::coordInRect(x, y, 0, 0, GameEngine::MAX_X, GameEngine::MAX_Y)){

        const double hsx = GameEngine::MAX_X / 2.0f;
        const double hsy = GameEngine::MAX_Y / 2.0f;

        QPointF *temp;
        //find intersection between player-enemy line and the edge of the screen
        temp = Util::lineIntersect(hsx, hsy, x, y, 0, 0, GameEngine::MAX_X, 0);
        if (temp == NULL) temp = Util::lineIntersect(hsx, hsy, x, y, GameEngine::MAX_X, 0, GameEngine::MAX_X, GameEngine::MAX_Y);
        if (temp == NULL) temp = Util::lineIntersect(hsx, hsy, x, y, 0, GameEngine::MAX_Y, GameEngine::MAX_X, GameEngine::MAX_Y);
        if (temp == NULL) temp = Util::lineIntersect(hsx, hsy, x, y, 0, 0, 0, GameEngine::MAX_Y);
        if (temp != NULL){
            //found intersection point, draw marker
            Util::drawRoundShape(temp->x(), temp->y(),
                                 //1000 is the distance the enemy has to be to be off by in order to get the biggest circle
                                 //10 is the minimum size circle
                                 //60 (50 + 10) is the maximum size circle
                                 Util::min(Util::distance(temp->x(), temp->y(), x, y), 1000)/1000.0f * 50 + 10,
                                 15, false, ResourceManager::getInstance()->getColour(ResourceManager::WHITE));
        }
        //clean up
        delete temp;
    }

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

void Enemy::drawFaded() const{
    //draw healer lightning
    if (currentEnemy != NULL && timerActive && type == ObjectManager::HEALER){
        Util::drawJaggedLine(x, y, currentEnemy->getX(), currentEnemy->getY(), clr);
    }
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

    //powerup (use the maxLife to dertermine value of powerup (harder = better powerup)
    if (type != ObjectManager::BULLET && qrand() % 3 == 0)
        ObjectManager::getInstance()->spawnPowerup(ObjectManager::HEALTH, x, y, x_vel, y_vel, maxLife);
}

void Enemy::findDirection(double x2, double y2, double x_tar2, double y_tar2){

    //calculating the x and y distance from enemy spawn and current player position
    double x_dist = abs(x_tar2 - x2);
    double y_dist = abs(y_tar2 - y2);

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
