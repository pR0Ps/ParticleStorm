/**
 * Header file for the Player class.
 *
 * Last modified by: Luke
 * Last modified on: March 4, 2012
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
#include "gameobject.h"

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

    //spacing of the particles to drop
    static const int PARTICLE_SPACING = 15;

    //max size of the player
    static const int MAX_DIAMETER = 24;

    //size of the rings around the player
    static const int RING_SIZE = 4;

    // Constructor/destructor.
    Player();
    ~Player();

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
    //maybe change to diameter if needed, but I think most calculations should use radius
    double getRadius () const {return (MAX_DIAMETER * getLifePercent())/2.0;}

    //score stuff
    long int getScore() const {return score;}
    void modScore(int amt, bool rel);

private:
    // Data members.
    // These represent the location of the player on the last call to update.
    double x_old, y_old;
    // Not sure what type to use for this yet - will likely use enum constants.
    // This may be unnecessary actually - may be simpler to just call the
    // useAbility function with the ability as an argument. The currently
    // selected abiliity will be determined in the update function.
    // currentAbility;
    int mana;

    //player score
    long int score;

    // Private member functions. The init function does not need to be
    // overridden and will likely be removed.
    /*
     * Modifies the player's mana by the specified amount. Adds the amount given
     * to the player's mana so if you want to reduce the player's mana then pass
     * a negative value to this function.
     *
     * Parameter: the amount to add to the player's mana.
     * Parameter: amount is reletive to current mana or not (set vs mod)
     */
    void modMana(int amount, bool rel = true);

    /*
     * Determines if the given mouse position is within the bounds of the OpenGL
     * canvas. Used to determine if the current location of the player's mouse
     * should be used to update the location of the avatar.
     *
     * Parameter: the position of the player's mouse, stored as a QPoint. This
     * should be given relative to the canvas widget, as opposed to in absolute
     * screen coordinates.
     *
     * Note: may need to take the size of the player's avatar into consideration
     * as well, at least until the player's movements can be used to move the
     * entire environment.
     */
    static bool isValidMousePos(const QPoint& pos);

    /*
     *
     */
    void useAbility() const; // not sure yet if this will modify the object

    /*
     *
     */
    void changeAbility();
};

#endif // PLAYER_H
