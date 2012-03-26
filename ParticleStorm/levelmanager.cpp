#include "levelmanager.h"

const double LevelManager::INITIAL_TTL;


LevelManager::LevelManager() {
    currLvl=1;
    this->ttl = INITIAL_TTL;
}

void LevelManager::startLevel(int lvl) {
    int temp = lvl*lvl;
    if (ttl <=0) {
        for (int i=0; i<(temp);i++) {
            ObjectManager::getInstance()->spawnEnemy(ObjectManager::SHOOTER, qrand()%GameEngine::MAX_X, qrand()%GameEngine::MAX_Y, ObjectManager::getInstance()->getPlayer()->getX(), ObjectManager::getInstance()->getPlayer()->getY());
        }
        for (int i=0; i<(temp*2);i++) {
            ObjectManager::getInstance()->spawnEnemy(ObjectManager::GRUNT, qrand()%GameEngine::MAX_X, qrand()%GameEngine::MAX_Y, ObjectManager::getInstance()->getPlayer()->getX(), ObjectManager::getInstance()->getPlayer()->getY());
        }
        for (int i=0; i<(temp*2);i++) {
            ObjectManager::getInstance()->spawnEnemy(ObjectManager::HEALER, qrand()%GameEngine::MAX_X, qrand()%GameEngine::MAX_Y, ObjectManager::getInstance()->getPlayer()->getX(), ObjectManager::getInstance()->getPlayer()->getY());
        }
        for (int i=0; i<(temp*2);i++) {
            ObjectManager::getInstance()->spawnEnemy(ObjectManager::TANK, qrand()%GameEngine::MAX_X, qrand()%GameEngine::MAX_Y, ObjectManager::getInstance()->getPlayer()->getX(), ObjectManager::getInstance()->getPlayer()->getY());
        }
        for (int i=0; i<(temp*3);i++) {
            ObjectManager::getInstance()->spawnEnemy(ObjectManager::SPRINTER, qrand()%GameEngine::MAX_X, qrand()%GameEngine::MAX_Y, ObjectManager::getInstance()->getPlayer()->getX(), ObjectManager::getInstance()->getPlayer()->getY());
        }
    }
}

void LevelManager::nextLevel() {
    currLvl++;
    startLevel(currLvl);
}

void LevelManager::update(double deltaTime) {
    if (isFinished() && ttl >= 0) {
        ttl -= deltaTime;
        //qDebug() << "dTime" << deltaTime;
        //qDebug() << "ttl" << ttl;
    }
    else if (isFinished() && ttl <= 0) {
        nextLevel();
    }

}

void LevelManager::draw() {
    string strLvl = "LEVEL: " + Util::doubleToString((double) currLvl,0,0);
    Util::drawString(strLvl, GameEngine::MAX_X/2, GameEngine::MAX_Y/2, ResourceManager::getInstance()->getTexture(ResourceManager::TEXT), true, true, 3,3);
}

bool LevelManager::isFinished () {
    if (ObjectManager::getInstance()->getNumObjects(ObjectManager::ENEMY)==0) {
        return true;
    }
    return false;
}

