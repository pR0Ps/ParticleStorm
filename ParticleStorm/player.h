/**
 * Header file for the Player class.
 *
 * Last modified by: Luke
 * Last modified on: March 29, 2012
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <QPoint>
#include "mainwindow.h"
#include "gameobject.h"
#include "enemy.h"

// Forward declarations needed for compilation.
class MainWindow;
class Enemy;

class Player : public GameObject {
public:
    // Global constants.
    // Note: if any of these constants are only used in one method then it would
    // probably be more appropriate to define them there instead of having them
    // accessible to the whole class. Also, may want to move these definitions
    // into private access space if they shouldn't be accessible by other
    // classes.
    static const int MAX_LIFE;
    static const int MAX_MANA;
    static const int RAM_DAMAGE;

    //max size of the player
    static const int MAX_DIAMETER;

    //size of the rings around the player
    static const int RING_SIZE;

    //spacing of the particles to drop
    static const int PARTICLE_SPACING;

    //time between accepting damage from enemies
    static const double MAX_COLLISION_BUFFER_TIME;
    static const double COLLISON_INDICATION_TIME;

    // Constants for mana regeneration.
    static const int MANA_REGEN_DELAY; // measured in seconds
    static const int MANA_REGEN_RATE; // per second

    // Enumeration for the player's special abilities.
    enum Ability {
        SPRAY,
        VORTEX,
        REPULSE,
        SHOCKWAVE,
        LIGHTNING,
        STORM
    };

    // The amount of time that must pass before the player's special ability can
    // be changed when the change ability button is held down continuously.
    static const double TIME_BETWEEN_CHG_ABILITY; // measured in seconds

    // Dummy values are used for the ability constants at the moment - these can
    // be tinkered with in the future to modify the game difficulty.

    // Constants for the lightning ability.
    static const int LIGHTNING_RANGE;
    static const int LIGHTNING_DPS; // DPS - damage per second
    static const int LIGHTNING_MANA_COST; // mana consumed per second
    // The ratio of life based on damage dealt that the player will be healed
    // for when the lightning ability hits an enemy.
    static const double LIGHTNING_HEAL_MODIFIER;
    // Don't draw the lightning effect if the enemy is closer than 10 pixels to
    // the player since the drawJaggedLine function may cause the program to
    // crash otherwise.
    static const int MIN_LIGHTNING_DRAW_DISTANCE; // used by storm as well

    // Constants for the spray ability.
    static const int SPRAY_PPS;
    static const int SPRAY_MANA_COST;
    // The intial speed to be given to particles spawned by the spray ability.
    static const int SPRAY_PARTICLE_SPEED;

    // Constants for the vortex ability.
    static const int VORTEX_MANA_COST;
    static const int VORTEX_PPS;
    // The distance from the player that particles will be spawned by the vortex
    // ability.
    static const int VORTEX_SPAWN_RAD;
    // The initial velocity to apply to the particles spawned by vortex.
    static const int VORTEX_SPAWN_VELOCITY;

    //constants for the shockwave ability
    static const int SHOCKWAVE_MANA_COST;
    static const int SHOCKWAVE_PPS;
    static const int SHOCKWAVE_INNER_RAD;
    static const int SHOCKWAVE_DPS;

    //constants for the repulse ability
    static const int REPULSE_RANGE;
    static const int REPULSE_DPS;
    static const int REPULSE_FORCE;
    static const int REPULSE_MANA_COST;
    static const double REPULSE_TIME;

    // Constants for the storm ability.
    static const int STORM_MANA_COST;
    static const int STORM_RANGE;
    static const unsigned int NUM_STORM_TARGETS;
    static const int STORM_DPS;

    // Constructor/destructor.
    Player();
    ~Player();

    // Accessors for the x_old and y_old data members.
    double getXOld() const { return x_old; }
    double getYOld() const { return y_old; }

    //resets the player
    void reset();

    // Pure virtual functions to override. These are analogous to abstract
    // methods in Java.
    // Note: do not use the virutal keyword in the implementation of these
    // functions.
    void update(double deltaTime);
    void drawNoFade() const;
    void drawFaded() const;

    //Have to override to do nothing
    void applyForce(double, double, double, double, double){}
    void pan(double, double){}
    void die(){}

    //get player properties
    int getMana() const {return mana;}
    int getMaxMana() const {return MAX_MANA;}
    float getManaPercent() const {return mana/(float)MAX_MANA;}
    double getRadius () const {return (MAX_DIAMETER * getLifePercent())/2.0;}

    //immunity for damage
    bool isImmune() const {return collisionBufferTime > 0;}
    void setImmune();

    //score stuff
    long int getScore() const {return score;}
    void modScore(int amt, bool rel);

    /*
     * Modifies the player's mana by the specified amount. Adds the amount given
     * to the player's mana so if you want to reduce the player's mana then pass
     * a negative value to this function.
     *
     * Parameters:
     * - the amount to add to the player's mana
     * - whether or not the amount should be relative to the current mana
     * (set vs mod)
     */
    void modMana(double amount, bool rel = true);

    /*
     * Returns a string representation of the player's currently selected
     * ability for display in the game HUD.
     */
    std::string getAbilityString() const;

private:
    // Data members.
    // These represent the location of the player on the last call to update.
    double x_old, y_old;
    double mana;

    //timers
    double collisionBufferTime;
    double hitDisplayTime;
    double repulseDisplayTime;

    //score
    long int score;

    // The amount of time remaining before the player's mana will start to
    // automatically regenerate. Mana will be regenerated when this reaches 0.
    double manaRegenTime;

    // The player's currently selected special ability. This should only be
    // assigned values from the ability enum.
    int currentAbility;
    // Variables for keeping track of how often change ability can be activated.
    bool chgAbilityActivatedOnLastUpdate;
    double timeSinceLastChgAbility; // measured in seconds

    // Used to determine when jagged lines should be drawn in the Player's draw
    // method for the lightning ability.
    Enemy* lightningTarget;
    // Similar to the lightningTarget attribute, this is used to determine when
    // jagged lines should be drawn to enemies hit by the storm abililty.
    std::vector<Enemy*> stormTargets;

    // Private member functions.

    //draw the player
    void drawPlayer() const;

    /*
     * Determines if the given mouse position is within the bounds of the OpenGL
     * canvas. Used to determine if the current location of the player's mouse
     * should be used to update the location of the avatar.
     *
     * Parameter: the position of the player's mouse, stored as a QPoint. This
     * should be given relative to the canvas widget, as opposed to in absolute
     * screen coordinates.
     */
    static bool isValidMousePos(const QPoint& pos);

    /*
     * Polls the user's input and performs the appropriate ability if one has
     * been activated.
     *
     * Parameters: the time since the last call to update, and pointers to the
     * current ObjectManager and MainWindow.
     */
    void performAbility(double deltaTime, ObjectManager* manager,
                        MainWindow* window);

    /*
     * Performs the drop particles ability. Particles are dropped along the line
     * between the player's position on the last call to update and their
     * current position.
     *
     * Parameters: the current ObjectManager.
     *
     * Note: the calculation performed does not depend on the time since the
     * last call to update but rather the distance that the player has
     * travelled. This encourages the player to move around while spawning
     * particles instead of staying still.
     */
    void dropParticles(ObjectManager* manager) const;

    /*
     * Applies force to all nearby enemies and star objects, pushing them away
     * from the player's current position.
     *
     * Parameter: the current ObjectManager.
     *
     * Note: force push and force pull are applied only to particles and stars.
     */
    void forcePush(ObjectManager* manager) const;

    /*
     * Applies force to all nearby enemies and stars, pulling them towards the
     * player's current position.
     *
     * Parameter: the current ObjectManager.
     */
    void forcePull(ObjectManager* manager) const;

    /*
     * Performs the player's currently selected special ability when the special
     * ability button is pressed.
     *
     * Parameters: the time since the last call to update and a pointer to the
     * current ObjectManager.
     */
    void useAbility(double deltaTime, ObjectManager* manager);

    /*
     * Contains the logic necessary to perform the lightning ability.
     *
     * Parameters: the time since the last call to update and a pointer to the
     * current ObjectManager.
     */
    void lightningAbility(double deltaTime, ObjectManager* manager);

    /*
     * Implements the spray abililty. Fires off several particles from the
     * player's current position at high speed in random directions. The
     * particles spawned are given an intial velocity, so in essence this is
     * just a combination of the drop particles and force push abilities rolled
     * into one.
     */
    void sprayAbility(double deltaTime, ObjectManager* manager);

    /*
     * Implements the vortex ability. Causes several particles to spawn and
     * rapidly spin around the player, damaging any enemies that get too close.
     */
    void vortexAbility(double deltaTime, ObjectManager* manager);

    /*
     * Creates a shield-like ring of particles around the player
     */
    void shockwaveAbility(double deltaTime, ObjectManager* manager);

    /*
     * Implements the shockwave ability. Emits a shockwave that damages enemies
     * and pushes them back.
     */
    void repulseAbility (double deltaTime, ObjectManager* manager);

    /*
     * Performs the storm abilitly. This is similar to the lightning abililty
     * except that it hits multiple targets and doesn't heal the player based
     * upon the amount of damage dealt.
     *
     * Parameters: the time since the last game update and a pointer to the
     * current Objectmanager.
     */
    void stormAbility(double deltaTime, ObjectManager* manager);

    /*
     * Iterates through the enemies currently alive in the game and targets the
     * enemies closest to the player. The number of targets selected is
     * controlled by the NUM_STORM_TARGETS constant.
     *
     * Parameter: a pointer to the current ObjectManager.
     *
     * Note: this function and insertEnemy could be generalized to find the
     * closest n GameObjects to the player for other purposes, instead of only
     * working for the storm ability.
     */
    void findStormTargets(ObjectManager* manager);

    /*
     * A helper function for findStormTargets. Attempts to insert the given
     * enemy into the list of storm targets based upon the distance between the
     * player and the enemy. Enemies are inserted into this list in
     * nondecreasing order of distance so that the closest enemies are targeted
     * by the storm ability instead of just enemies within range of it.
     *
     * Parameters: a pointer to the enemy to insert into the list of storm
     * targets.
     */
    void insertEnemy(Enemy* enemy);

    /*
     * Advances the player's currently selected special ability to the next one
     * in the sequence when the change ability button is pressed. The ability
     * can be changed a limited number of times per second when the change
     * ability button is held down.
     *
     * Parameter: the time since the last call to update.
     */
    void changeAbility(double deltaTime);

    /*
     * Draws the lightning effect when the ability has been activated, so long
     * as the target enemy is not too close to the player.
     */
    void drawLightning() const;

    /*
     * Draws a lightning effect to each of the enemies targeted by the storm
     * ability, but only if they are a minimum distance away from the player.
     */
    void drawStorm() const;

    /*
     * Resets the player's remaining time until mana regeneration to the delay.
     * Should be called whenever a special ability is performed IF it actually
     * consumes mana.
     */
    void resetManaRegenTime() { manaRegenTime = MANA_REGEN_DELAY; }

    /*
     * Automatically regenerates a certain amount of the player's mana when
     * enough time has passed since a special ability was last used.
     *
     * Parameter: the time (in seconds) since the last game update.
     *
     * Note: this function will still be called while in Zen Mode, but the call
     * to modMana will do nothing.
     */
    void regenMana(double deltaTime);
};

#endif // PLAYER_H
