#include "levelmanager.h"

levelmanager::levelmanager() {}

void levelmanager::startLevel() {

}

bool levelmanager::isFinished () {
    if (ObjectManager::getInstance()->getNumObjects(ObjectManager::ENEMY)==0) {
        return true;
    }
    return false;
}

