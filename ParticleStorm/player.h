/**
 * Header file for the Player class.
 *
 * Last modified by: Luke
 * Last modified on: March 25, 2012
 *
 * Notes:
 * - will add more documentation as I implement functions
 *
 * Todo:
 * - implement draw and update to get a basic prototype up and running
 * - update should be fairly simple, at least until abilities are involved. Just
 * need to update the player's x and y coordinates with the location of the
 * mouse.
 * - draw should also be fairly simple initially. Draw a simple circle to
 * represent the player's avatar (will need some OpenGL for this).
 * - applyForce and doCollision will be more complicated... although the
 * majority of collision detection will be performed in a more abstract class
 *
 * Questions:
 * - how should abilities be represented? Could just use a set of enum constants
 * to identify them and have the Player class handle the rest, but this may be
 * inadequate. A better approach may be to add an inner class within Player to
 * represent them.
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <QPoint>
#include "mainwindow.h"
#include "gameobject.h"
#include "enemy.h"
using namespace std;

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
    static const int MAX_LIFE = 1000;
    static const int MAX_MANA = 1000;
    static const int RAM_DAMAGE = 10;

    //spacing of the particles to drop
    static const int PARTICLE_SPACING = 15;

    //max size of the player
    static const int MAX_DIAMETER = 24;

    //size of the rings around the player
    static const int RING_SIZE = 4;

    // Enumeration for the player's special abilities.
    enum ability {
        VORTEX,
        SPRAY,
        REPULSE,
        LIGHTNING,
        SHOCKWAVE
    };

    // The amount of time that must pass before the player's special ability can
    // be changed when the change ability button is held down continuously.
    static const double TIME_BETWEEN_CHG_ABILITY = 0.5; // measured in seconds

    // Constants for the lightning ability.
    static const int LIGHTNING_RANGE = 250;
    static const int LIGHTNING_DPS = 50; // DPS - damage per second
    static const int LIGHTNING_MANA_COST = 50; // mana consumed per second
    // Don't draw the lightning effect if the enemy is closer than 10 pixels to
    // the player since the drawJaggedLine function may cause the program to
    // crash otherwise.
    static const int MIN_LIGHTNING_DRAW_DISTANCE = 10;

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
    void draw() const;

    //Have to override to do nothing
    void applyForce(double x, double y, double mag){}
    void pan(double x, double y){}
    void die(){}

    //get player properties
    int getMana() const {return mana;}
    int getMaxMana() const {return MAX_MANA;}
    float getManaPercent() const {return mana/(float)MAX_MANA;}
    //maybe change to diameter if needed, but I think most calculations should
    //use radius
    double getRadius () const {return (MAX_DIAMETER * getLifePercent())/2.0;}

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
    string getAbilityString() const;

private:
    // Data members.
    // These represent the location of the player on the last call to update.
    double x_old, y_old;
    int mana;
    long int score;
    // The player's currently selected special ability. This should only be
    // assigned values from the ability enum.
    int currentAbility;
    // Variables for keeping track of how often change ability can be activated.
    bool chgAbilityActivatedOnLastUpdate;
    double timeSinceLastChgAbility; // measured in seconds

    // Used to determine when jagged lines should be drawn in the Player's draw
    // method for the lightning ability.
    Enemy* lightningTarget;

    // Private member functions.
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
};

#endif // PLAYER_H
