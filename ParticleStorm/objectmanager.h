#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include "gameobject.h"
#include "util.h"
#include "enemy.h"
#include "particle.h"
#include "player.h"
#include "powerup.h"
#include "shrapnel.h"
#include "star.h"

#include <vector>

class ObjectManager;
class Enemy; // forward declaration of Enemy class (needed for compilation)
class Player; // same for the Player class

class ObjectManager{
public:
    ObjectManager();
    ~ObjectManager();

    //constants (redeclare in implementation)
    static const int MAX_PARTICLES = 700;
    static const int INIT_ENEMIES = 100;
    static const int INIT_POWERUPS = 30;
    static const int MAX_SHRAPNEL = 40;
    static const int MAX_STARS = 250;
    static const int RESIZE_AMT = 20;

    //object types
    enum ObjectType{
        PLAYER,
        PARTICLE,
        ENEMY,
        POWERUP,
        SHRAPNEL,
        STAR
    };
    //enemy types
    enum EnemyType{
        GRUNT, //must be first
        HEALER,
        TANK,
        SPRINTER,
        SHOOTER, //must be second-last
        BULLET //must be last
    };
    //powerup types
    enum PowerupType{
        HEALTH,
        MANA
    };

    //resets all the objects
    void reset();

    //convenience function for drawing everything
    void drawAll(bool faded);

    //apply to all the objects in the OM individually
    void drawNoFade(ObjectType t);
    void drawFaded(ObjectType t);
    void pan(ObjectType t, double x, double y);
    void update(ObjectType t, double deltaTime);
    void applyForce(ObjectType t, double x, double y, double mag, double range, double dissipation);

    //get number of objects
    unsigned int getNumObjects(ObjectType t);

    //player stuff
    const Player* getPlayer(){return player;}
    void modPlayerLife(int amt, bool rel = true);
    void modPlayerMana(int amt, bool rel = true);
    void modPlayerScore(int amt, bool rel = true);

    //get unused object to modify
    GameObject* getUnused(ObjectType t);

    //get a list of objects within a certain range
    std::vector<GameObject*>* getObjectsInRange(ObjectType t, double x, double y, double range);

    //do collisions
    void doEnemyParticleCollisions();
    void doPlayerEnemyCollisions();
    void doPlayerPowerupCollisions();

    //spawning stuff
    void spawnParticle(double x, double y, double x_vel, double y_vel);
    void spawnParticle(double x, double y);
    void spawnPowerup (PowerupType t, double x, double y, double x_vel, double y_vel, int value);
    void spawnEnemy(EnemyType t, double x, double y, double x_tar, double y_tar);
    void spawnEnemy(EnemyType t, double x, double y);
    void spawnEnemy(EnemyType t, QPoint p);
    void spawnEnemy(QPoint p);
    void spawnShrapnel (double x, double y, double x_vel, double y_vel, int num, double len, const QColor *clr);

    //enemy stuff
    /*
     * Finds the closest enemy relative to the x and y screen coordinates. The
     * minDistance and maxDistance parameters form bounds on how close or far
     * away the Enemy must be to be considered "close" to the given coordinates.
     *
     * Note: minDistance should be greater than 0 when the coordinates given are
     * of an enemy, otherwise the closest enemy will be determined to be the
     * enemy itself.
     */
    Enemy* getClosestEnemy(double x, double y, double minDistance = 1, double maxDistance = 1000);

    //get the object manager instance
    static ObjectManager* getInstance(){return instance;}

private:
    //holds all the objects
    Player *player;
    std::vector<GameObject*> *particles;
    std::vector<GameObject*> *enemies;
    std::vector<GameObject*> *powerups;
    std::vector<GameObject*> *shrapnel;
    std::vector<GameObject*> *stars;

    //self-reference
    static ObjectManager* instance;

    //want to create in sequential order always
    unsigned int cur_particle;
    unsigned int cur_shrapnel;

    //returns the address of the proper vector
    std::vector<GameObject*>& getVector(ObjectType t);

    //manage the object pools;
    void initPool(ObjectType t, unsigned int numObjects);
    void extendPool(ObjectType t, unsigned int extra);

    //sets all the objects as inactive
    void deactivateAll(ObjectType t);
    bool playerCollision(double, double, double, double, double, double,double);
};

#endif // OBJECTMANAGER_H
