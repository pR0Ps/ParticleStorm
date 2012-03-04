/**
 * Header file for the Player class.
 *
 * Last modified by: Luke
 * Last modified on: March 4, 2012
 *
 * Notes:
 * - will add more documentation as I implement methods
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

#include "gameobject.h"

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
    // The smallest possible size of the player's avatar, when not modified by
    // their current life. Represented in pixels.
    static const int BASE_SIZE; // may want to store this in a double
    // The colour of the player's avatar. It may be better to specify an RGB
    // value so that the colour is more unique.
    static const Qt::GlobalColor AVATAR_COLOUR;

    // Constructors. Probably won't need a constructor that takes an object
    // manager as a parameter.
    /*
     * Default constructor.
     */
    Player();

    /*
     * Destructor
     */
    ~Player();

    // Pure virtual functions to override. These are analogous to abstract
    // methods in Java.
    // Note: do not use the virutal keyword in the implementation of these
    // functions.
    virtual void update();
    virtual void draw() const;
    virtual void doCollision(GameObject* other);
    virtual void applyForce(double x, double y, double mag);
    virtual void die(); // may or may not need to override this

private:
    // Data members.
    // These represent the location of the player on the last call to update.
    double x2, y2;
    // Not sure what type to use for this yet - will likely use enum constants.
    // This may be unnecessary actually - may be simpler to just call the
    // useAbility function with the ability as an argument. The currently
    // selected abiliity will be determined in the update function.
    // currentAbility;
    int mana;

    // The size of the player's avatar (represented by a circle). This shrinks
    // in size as the player's life decreases to aid the player in recovering.
    // This data member may not be needed actually, as the avatar's size depends
    // on the player's life.
    int size;

    // Private member functions. The init function does not need to be
    // overridden and will likely be removed.
    /*
     * Modifies the player's mana by the specified amount. Adds the amount given
     * to the player's mana so if you want to reduce the player's mana then pass
     * a negative value to this function.
     *
     * Parameter: the amount to add to the player's mana.
     */
    void modMana(int amount);

    /*
     * Updates the size of the player's avatar, based upon the player's
     * remaining life. The size of the avatar will decrease as the player's life
     * decreases. This should be called in the constructor to initialize the
     * size data member and in the Player's update function, since the player's
     * life may have changed.
     *
     * Note: I have inlined this function since it performs a fairly simple
     * calculation. Apparently inlined functions must be defined in a header
     * file if it will be called in other .cpp files, but it shouldn't matter in
     * this case since it's a private member function.
     */
    void updateSize();

    void updateColour();

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
