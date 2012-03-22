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
class ObjectManager{
public:
    ObjectManager();
    ~ObjectManager();

    //constants (redeclare in implementation)
    static const int MAX_PARTICLES = 200;
    static const int MAX_ENEMIES = 20;
    static const int MAX_POWERUPS = 20;
    static const int MAX_SHRAPNEL = 40;
    static const int MAX_STARS = 250;

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
        GRUNT,
        HEALER,
        TANK,
        SPRINTER,
        SHOOTER,
        BULLET
    };
    //powerup types
    enum PowerupType{
        HEALTH,
        MANA
    };

    //resets all the objects
    void reset();

    //apply to all the objects in the OM individually
    void draw(ObjectType t);
    void pan(ObjectType t, double x, double y);
    void update(ObjectType t, double deltaTime);
    void applyForce(ObjectType t, double x, double y, double mag);

    //initilize the object pool of type t;
    void initPool(ObjectType t, unsigned int numObjects);

    //get number of objects
    unsigned int getNumObjects(ObjectType t);

    //player stuff
    const Player* getPlayer(){return player;}
    void modPlayerLife(int amt, bool rel = true){player->modLife(amt, rel);}
    void modPlayerScore(int amt, bool rel = true){player->modScore(amt, rel);}

    //get unused object to modify
    GameObject* getUnused(ObjectType t);

    //do collisions
    void doEnemyParticleCollisions();
    void doPlayerEnemyCollisions();
    void doPlayerPowerupCollisions();

    //spawning stuff
    void spawnParticle(double x, double y, double x_vel, double y_vel);
    void spawnParticle(double x, double y);
    void spawnPowerup (PowerupType t, double x, double y, double x_vel, double y_vel, int value);
    void spawnEnemy(EnemyType t, double x, double y, double x_tar = 0, double y_tar = 0);
    void spawnShrapnel (double x, double y, double x_vel, double y_vel, double len, const QColor *clr);

    //enemy stuff
    //Enemy* getClosestEnemy(double x, double y, double min_dist = 1);

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

    //particle number
    //want to create in sequential order always
    unsigned int cur_particle;

    //returns the address of the proper vector
    std::vector<GameObject*>& getVector(ObjectType t);

    //sets all the objects as inactive
    void deactivateAll(ObjectType t);
};

#endif // OBJECTMANAGER_H
