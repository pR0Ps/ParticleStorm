/**
 * Implementation file for the Player class.
 *
 * Last modified by: Luke
 * Last modified on: March 4, 2012
 */

#include "player.h"
#include "glcanvas.h"

// Declaration of constants. Some of these can be modified to alter the
// difficulty of the gameplay.
const int Player::MAX_LIFE(100);
const int Player::MAX_MANA(100);
const int Player::BASE_SIZE(50);
const Qt::GlobalColor Player::AVATAR_COLOUR(Qt::red);


// Implementation of constructors.

// Some data members of the Player class do not need to initialized:
// - x_vel and y_vel -> the avatar's velocity is determined entirely by the
// player's mouse movements, so these are not needed
// manager -> the Player object will likely not need an ObjectManager, and this
// data member may be removed from the GameObject class anyways
Player::Player():
    GameObject(), // call parent class constructor - initializes inUse to true
    mana(MAX_MANA) {
    // *This must be defined in the body of the constructor, since it is
    // inherited from the GameObject class (see the C++ Programming Tips page on
    // the Wiki).
    life = MAX_LIFE;
    updateSize(); // initialize the size of the player's avatar

    // Initialize the player's position to the middle of the screen. MAX_X and
    // MAX_Y are defined in glcanvas.h.
    // *Actually, may want to intialize this to the coordinates of the mouse,
    // if this information is available when the screen is first drawn.
    // may need to change this later to take the avatar's size into
    // consideration
    x = GLCanvas::MAX_X / 2;
    y = GLCanvas::MAX_Y / 2;
    // Dummy values for the previous position of the avatar on the last call to
    // update. This will be initialized to the correct value on the first call
    // to update.
    x2 = x;
    y2 = y;
    updateColour();
}

//free up allocated objects
Player::~Player(){

}


// Implementation of pure virtual functions.

void Player::update() {

}

void Player::draw() const {

}

void Player::doCollision(GameObject* other) {

}

void Player::applyForce(double x, double y, double mag) {

}

void Player::die() {

}


// Implementation of private member functions.

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

// The current calculation used (unless the value of some constants are changed)
// for the size of the player's avatar is: 50 pixels + 1 pixel for every 2 life
// points the player has. So the avatar's size can range from 50 to 100 pixels,
// and the avatar will only have the smallest size (50 pixels) when the player
// has 1 life left.
inline void Player::updateSize() {
    // Should use descriptive constants here instead of literal values.
    size = BASE_SIZE + life / 2 * 1;
}

void Player::updateColour(){
    clr = new QColor(AVATAR_COLOUR);
}

void Player::useAbility() const {

}

void Player::changeAbility() {

}
