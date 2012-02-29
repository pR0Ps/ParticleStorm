/**
 * Header file for the Player class.
 *
 * Last modified by: Luke
 * Last modified on: February 29, 2012
 *
 * Notes:
 * - will add more documentation as I implement methods
 *
 * Todo:
 * - finish implementing constructor (may need 2)
 * - implement draw and update to get a basic prototype up and running
 * - update should be fairly simple, at least until abilities are involved. Just
 * need to update the player's x and y coordinates with the location of the
 * mouse
 * - draw should also be fairly simple initially. Draw a simple circle to
 * represent the player's avatar (will need some OpenGL for this).
 * - applyForce and doCollision will be more complicated...
 *
 * Questions:
 * - how should abilities be represented? Could just use a set of enum constants
 * to identify them and have the Player class handle the rest, but this may be
 * inadequate. A better approach may be to add an inner class within Player to
 * represent them.
 */

#ifndef PLAYER_H
#define PLAYER_H

#include "gameobject.h"

class Player : public GameObject {
public:
    // Constants.
    // Note: if any of these constants are only used in one method then it would
    // probably be more appropriate to define them there instead of having them
    // accessible to the whole class. Also, may want to move these definitions
    // into private access space if they shouldn't be accessible by other
    // classes.
    static const int MAX_LIFE;
    static const int MAX_MANA;

    // Constructors.
    /*
     * Default constructor.
     */
    Player();

    // May need a constructor that takes an ObjectManager as a parameter.

    // Pure virtual functions to override. These are analogous to abstract
    // methods in Java.
    // Note: do not use the virutal keyword in the implementation of these
    // functions.
    virtual void draw() const;
    virtual void update();
    virtual void applyForce(double x, double y, double mag);
    virtual void die(); // may or may not need to override this
    virtual void doCollision(GameObject* other);

private:
    // Attributes.
    double x2, y2; // what are these needed for?
    // Not sure what type to use for this yet - will likely use enum constants.
    // currentAbility;
    int mana;

    // The size of the player's avatar (represented by a circle). This shrinks
    // in size as the player's life decreases to aid the player in recovering.
    // This attribute may not be needed actually, as the avatar's size depends
    // on the player's life.
    int size;

    // Private methods.
    /*
     * Modifies the player's mana by the specified amount. Adds the amount given
     * to the player's mana so if you want to reduce the player's mana then pass
     * a negative value to this method.
     *
     * Parameter: the amount to add to the player's mana.
     */
    void modMana(int amount);

    /*
     *
     */
    void useAbility() const; // not sure yet if this will modify the object

    /*
     *
     */
    void changeAbility();

    virtual void init(); // is this method really needed?
};

#endif // PLAYER_H
