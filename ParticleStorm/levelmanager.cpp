#include "levelmanager.h"

LevelManager::LevelManager() {currLvl=1;}

void LevelManager::startLevel() {
    int temp = currLvl*currLvl;
    /*for (int i=0; i<(temp);i++) {
        ObjectManager::getInstance()->spawnEnemy(ObjectManager::SHOOTER, qrand()%GameEngine::MAX_X, qrand()%GameEngine::MAX_Y, ObjectManager::getInstance()->getPlayer()->getX(), ObjectManager::getInstance()->getPlayer()->getY());
    }*/
    for (int i=0; i<(temp*2);i++) {
        ObjectManager::getInstance()->spawnEnemy(ObjectManager::GRUNT, qrand()%GameEngine::MAX_X, qrand()%GameEngine::MAX_Y, ObjectManager::getInstance()->getPlayer()->getX(), ObjectManager::getInstance()->getPlayer()->getY());
    }/*
    for (int i=0; i<(temp*10);i++) {
        ObjectManager::getInstance()->spawnEnemy(ObjectManager::HEALER, qrand()%GameEngine::MAX_X, qrand()%GameEngine::MAX_Y, ObjectManager::getInstance()->getPlayer()->getX(), ObjectManager::getInstance()->getPlayer()->getY());
    }
    for (int i=0; i<(temp*2);i++) {
        ObjectManager::getInstance()->spawnEnemy(ObjectManager::TANK, qrand()%GameEngine::MAX_X, qrand()%GameEngine::MAX_Y, ObjectManager::getInstance()->getPlayer()->getX(), ObjectManager::getInstance()->getPlayer()->getY());
    }
    for (int i=0; i<(temp*3);i++) {
        ObjectManager::getInstance()->spawnEnemy(ObjectManager::SPRINTER, qrand()%GameEngine::MAX_X, qrand()%GameEngine::MAX_Y, ObjectManager::getInstance()->getPlayer()->getX(), ObjectManager::getInstance()->getPlayer()->getY());
    }*/
}

bool LevelManager::isFinished () {
    if (ObjectManager::getInstance()->getNumObjects(ObjectManager::ENEMY)==0) {
        currLvl++;
        return true;
    }
    return false;
}

