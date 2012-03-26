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
    enemiesStarted = false;

    //init timers
    text_ttl = TEXT_DISPLAY_TIME;
    enemyCheck_ttl = ENEMY_CHECK_RATE;
}

void LevelManager::nextLevel() {
    startLevel(currType, currLvl + 1);
}

void LevelManager::startEnemies(){
    ObjectManager *om = ObjectManager::getInstance();

    //start the enemies for the level
    if (currType == NONSTOP){
        for (int i = 0; i < std::min((currLvl * ENEMY_GROWTH) + MIN_ENEMIES, MAX_ENEMIES) ; i++) {
            om->spawnEnemy(qrand() % GameEngine::MAX_X, qrand() % GameEngine::MAX_Y);
        }
    }
    else if (currType == LEVELED){
        //make some legit levels (use case stmts without breaks to stack)
    }
    else if (currType == ZEN){
        //zen mode, no enemies ever
    }
    enemiesStarted = true;
}

bool LevelManager::levelFinished () const {
    return levelDone;
}

void LevelManager::update(double deltaTime) {
    //run timers
    if (text_ttl > 0) text_ttl -= deltaTime;
    if (enemyCheck_ttl > 0) enemyCheck_ttl -= deltaTime;

    //check for enemy spawning
    if (!enemiesStarted && text_ttl <= 0){
        startEnemies();
    }

    //check for the end of the level
    if (enemiesStarted && enemyCheck_ttl <= 0) {
        enemyCheck_ttl = ENEMY_CHECK_RATE;
        levelDone = ObjectManager::getInstance()->getNumObjects(ObjectManager::ENEMY) == 0;
    }
}

void LevelManager::drawNoFade() const{
    if (text_ttl > 0){
        Util::drawString("LEVEL: " + Util::doubleToString((double) currLvl,0,0), GameEngine::MAX_X/2, GameEngine::MAX_Y/2, ResourceManager::getInstance()->getTexture(ResourceManager::TEXT), true, true, 3,3);
    }
}

