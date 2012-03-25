#include "levelmanager.h"

LevelManager::LevelManager() {currLvl=1;}

void LevelManager::startLevel() {

    //ObjectManager::getInstance()->spawnEnemy(ObjectManager::GRUNT, 500, 100, ObjectManager::getInstance()->getPlayer()->getX(), ObjectManager::getInstance()->getPlayer()->getY());
    //ObjectManager::getInstance()->spawnEnemy(ObjectManager::HEALER, 500, 100, ObjectManager::getInstance()->getPlayer()->getX(), ObjectManager::getInstance()->getPlayer()->getY());
    //ObjectManager::getInstance()->spawnEnemy(ObjectManager::TANK, 500, 100, ObjectManager::getInstance()->getPlayer()->getX(), ObjectManager::getInstance()->getPlayer()->getY());
    //ObjectManager::getInstance()->spawnEnemy(ObjectManager::SPRINTER, 500, 100, ObjectManager::getInstance()->getPlayer()->getX(), ObjectManager::getInstance()->getPlayer()->getY());

    for (int i=0; i<10;i++) {
        int tempx = qrand()%GameEngine::MAX_X;
        int tempy = qrand()%GameEngine::MAX_Y;
        ObjectManager::getInstance()->spawnEnemy(ObjectManager::SHOOTER, tempx, tempy, ObjectManager::getInstance()->getPlayer()->getX(), ObjectManager::getInstance()->getPlayer()->getY());
    }
}

bool LevelManager::isFinished () {
    if (ObjectManager::getInstance()->getNumObjects(ObjectManager::ENEMY)==0) {
        currLvl++;
        return true;
    }
    return false;
}

