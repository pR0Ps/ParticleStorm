#include "levelmanager.h"

const double LevelManager::TEXT_DISPLAY_TIME = 5;
const double LevelManager::ENEMY_CHECK_RATE = 5;
const int LevelManager::MAX_ENEMIES = 70;
const int LevelManager::MIN_ENEMIES = 5;
const int LevelManager::ENEMY_GROWTH = 3;
const int LevelManager::ENEMY_SPAWN_DIST = 300;
const int LevelManager::MAX_LEVEL = 25;

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
            om->spawnEnemy(randomOffscreenPos());
        }
    }
    else if (currType == LEVELED){
        //play the last level over and over if the gameengine doesn't stop the game properly
        int tempLvl = std::min(MAX_LEVEL, currLvl);

        //stacked levels (each loads enemies from all the levels below it as well)
        switch(tempLvl){
        case 25:
            om->spawnEnemy(ObjectManager::SHOOTER, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::SHOOTER, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::SPRINTER, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::SPRINTER, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::TANK, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::TANK, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::HEALER, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::HEALER, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::GRUNT, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::GRUNT, randomOffscreenPos());
        case 24:
            om->spawnEnemy(ObjectManager::TANK, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::TANK, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::SPRINTER, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::SPRINTER, randomOffscreenPos());
        case 23:
            om->spawnEnemy(ObjectManager::HEALER, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::HEALER, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::HEALER, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::HEALER, randomOffscreenPos());
        case 22:
            om->spawnEnemy(ObjectManager::GRUNT, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::GRUNT, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::GRUNT, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::GRUNT, randomOffscreenPos());
        case 21:
            om->spawnEnemy(ObjectManager::TANK, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::TANK, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::SPRINTER, randomOffscreenPos());
        case 20:
            om->spawnEnemy(ObjectManager::SHOOTER, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::SHOOTER, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::TANK, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::GRUNT, randomOffscreenPos());
        case 19:
            om->spawnEnemy(ObjectManager::HEALER, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::HEALER, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::HEALER, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::HEALER, randomOffscreenPos());
        case 18:
            om->spawnEnemy(ObjectManager::GRUNT, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::GRUNT, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::GRUNT, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::GRUNT, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::GRUNT, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::GRUNT, randomOffscreenPos());
        case 17:
            om->spawnEnemy(ObjectManager::SPRINTER, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::SPRINTER, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::TANK, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::TANK, randomOffscreenPos());
        case 16:
            om->spawnEnemy(ObjectManager::SHOOTER, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::SHOOTER, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::SHOOTER, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::SHOOTER, randomOffscreenPos());
        case 15:
            om->spawnEnemy(ObjectManager::TANK, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::TANK, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::GRUNT, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::GRUNT, randomOffscreenPos());
        case 14:
            om->spawnEnemy(ObjectManager::SPRINTER, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::SPRINTER, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::SHOOTER, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::SHOOTER, randomOffscreenPos());
        case 13:
            om->spawnEnemy(ObjectManager::HEALER, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::HEALER, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::SPRINTER, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::SPRINTER, randomOffscreenPos());
        case 12:
            om->spawnEnemy(ObjectManager::TANK, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::TANK, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::TANK, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::TANK, randomOffscreenPos());
        case 11:
            om->spawnEnemy(ObjectManager::GRUNT, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::GRUNT, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::GRUNT, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::GRUNT, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::GRUNT, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::GRUNT, randomOffscreenPos());
        case 10:
            om->spawnEnemy(ObjectManager::HEALER, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::HEALER, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::HEALER, randomOffscreenPos());
        case 9:
            om->spawnEnemy(ObjectManager::SPRINTER, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::SPRINTER, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::SPRINTER, randomOffscreenPos());
        case 8:
            om->spawnEnemy(ObjectManager::TANK, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::TANK, randomOffscreenPos());
        case 7:
            om->spawnEnemy(ObjectManager::HEALER, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::HEALER, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::HEALER, randomOffscreenPos());
        case 6:
            om->spawnEnemy(ObjectManager::GRUNT, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::GRUNT, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::GRUNT, randomOffscreenPos());
        case 5:
            om->spawnEnemy(ObjectManager::HEALER, randomOffscreenPos());
        case 4:
            om->spawnEnemy(ObjectManager::SPRINTER, randomOffscreenPos());
        case 3:
            om->spawnEnemy(ObjectManager::SHOOTER, randomOffscreenPos());
            om->spawnEnemy(ObjectManager::SHOOTER, randomOffscreenPos());
        case 2:
            om->spawnEnemy(ObjectManager::TANK, randomOffscreenPos());
        case 1:
            om->spawnEnemy(ObjectManager::GRUNT, randomOffscreenPos());
        }
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
    if (enemiesStarted && currType != ZEN && enemyCheck_ttl <= 0) {
        enemyCheck_ttl = ENEMY_CHECK_RATE;
        levelDone = ObjectManager::getInstance()->getNumObjects(ObjectManager::ENEMY) == 0;
    }
}

void LevelManager::drawNoFade() const{
    if (text_ttl > 0){
        if (currType == NONSTOP){
            Util::drawString(currLvl > 1 ? "NEXT WAVE!" : "FIRST WAVE!", GameEngine::MAX_X/2, GameEngine::MAX_Y/2, ResourceManager::getInstance()->getTexture(ResourceManager::TEXT), true, true, 3,3);
        }
        else if (currType == LEVELED){
            Util::drawString("LEVEL: " + Util::doubleToString((double) currLvl,0,0), GameEngine::MAX_X/2, GameEngine::MAX_Y/2, ResourceManager::getInstance()->getTexture(ResourceManager::TEXT), true, true, 3,3);
        }
        else if (currType == ZEN){
            Util::drawString("ZEN MODE", GameEngine::MAX_X/2, GameEngine::MAX_Y/2, ResourceManager::getInstance()->getTexture(ResourceManager::TEXT), true, true, 3,3);
        }
    }
}

//spawning enemies offscreen
QPoint LevelManager::randomOffscreenPos(){
    return Util::randCoordOnRect(-ENEMY_SPAWN_DIST, -ENEMY_SPAWN_DIST, GameEngine::MAX_X + ENEMY_SPAWN_DIST, GameEngine::MAX_Y + ENEMY_SPAWN_DIST);
}

