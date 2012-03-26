#include "levelmanager.h"

const double LevelManager::TEXT_DISPLAY_TIME;
const double LevelManager::ENEMY_CHECK_RATE;
const int LevelManager::MAX_ENEMIES;
const int LevelManager::MIN_ENEMIES;
const int LevelManager::ENEMY_GROWTH;

LevelManager::LevelManager() {
}

void LevelManager::startLevel(LevelType t, int lvl) {
    currType = t;
    currLvl = lvl;
    levelDone = false;

    //init timers
    text_ttl = TEXT_DISPLAY_TIME;
    enemyCheck_ttl = ENEMY_CHECK_RATE;

    ObjectManager *om = ObjectManager::getInstance();

    //start the enemies for the level
    if (t == INFINITE){
        for (int i = 0; i < std::min((currLvl * ENEMY_GROWTH) + MIN_ENEMIES, MAX_ENEMIES) ; i++) {
            om->spawnEnemy(qrand() % GameEngine::MAX_X, qrand() % GameEngine::MAX_Y);
        }
    }
    else if (t == LEVELED){
        //make some legit levels (use case stmts without breaks to stack)
    }
    else if (t == ZEN){
        //zen mode, no enemies ever
    }
}

void LevelManager::nextLevel() {
    startLevel(currType, currLvl + 1);
}

void LevelManager::update(double deltaTime) {
    //run timers
    if (text_ttl > 0) text_ttl -= deltaTime;
    if (enemyCheck_ttl > 0) enemyCheck_ttl -= deltaTime;

    //check for the end of the level
    if (enemyCheck_ttl <= 0) {
        enemyCheck_ttl = ENEMY_CHECK_RATE;
        levelDone = ObjectManager::getInstance()->getNumObjects(ObjectManager::ENEMY) == 0;
    }
}

void LevelManager::drawNoFade() const{
    if (text_ttl > 0){
        Util::drawString("LEVEL: " + Util::doubleToString((double) currLvl,0,0), GameEngine::MAX_X/2, GameEngine::MAX_Y/2, ResourceManager::getInstance()->getTexture(ResourceManager::TEXT), true, true, 3,3);
    }
}

bool LevelManager::levelFinished () const {
    return levelDone;
}

