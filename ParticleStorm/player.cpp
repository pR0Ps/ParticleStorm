/**
 * Implementation file for the Player class.
 *
 * Last modified by: Luke
 * Last modified on: February 29, 2012
 */

#include "player.h"

// Declaration of constants. (May want to do this in the header file instead.)
// These two constants can be modified to alter the difficulty of the gameplay
// (at the start of the game at least).
const int Player::MAX_LIFE = 100;
const int Player::MAX_MANA = 100;


// Implementation of constructors.

Player::Player():
    GameObject(), // call parent class constructor - initializes inUse to true

    // **Cannot set this attribute because it has private access in GameObject.
    // life(MAX_LIFE),

    mana(MAX_MANA)

    // Still need to initialize:
    // - x and y -> center the player's avatar on the screen initially (need to
    // do this in the body of the constructor)
    // - x_vel and y_vel -> the movement speed?
    // - clr -> the avatar's colour for the draw method (I presume)
    // - the object manager?
    // - x2 and y2?
    // - the size of the player's avatar?
{}


// Implementation of pure virtual methods.

void Player::draw() const {

}

void Player::update() {

}

void Player::applyForce(double x, double y, double mag) {

}

void Player::die() {

}

void Player::doCollision(GameObject* other) {

}


// Implementation of private methods.

void Player::modMana(int amount) {
    // Perform bounds checking on what the new value of mana would be.
    if (mana + amount > MAX_MANA)
        mana = MAX_MANA;
    else if (mana + amount < 0)
        mana = 0;
    else
        // the amount can be added to the player's mana without exceeding the
        // allowable limits
        mana += amount;
}

void Player::useAbility() const {

}

void Player::changeAbility() {

}

// May or may not need to override this.
void Player::init() {

}
