/**
 * Implementation file for the Player class.
 *
 * Last modified by: Luke
 * Last modified on: March 4, 2012
 */

#include "player.h"
#include "gameengine.h"
#include "mainwindow.h"


// Declaration of constants. Some of these can be modified to alter the
// difficulty of the gameplay.
const int Player::MAX_LIFE;
const int Player::MAX_MANA;
const int Player::MAX_DIAMETER;
const int Player::RING_SIZE;

// Implementation of constructor and destructor.

// Some data members of the Player class do not need to initialized:
// - x_vel and y_vel -> the avatar's velocity is determined entirely by the
// player's mouse movements, so these are not needed
// inUse also doesn't matter, player is always in use
Player::Player(): GameObject(){
    // call parent class constructor

    mana = MAX_MANA;
    maxLife = MAX_LIFE;

    reset();
}

Player::~Player() {
    // Base class destructors are automatically called, so this does not need to
    // be explicitly called here.
    // may need to delete more data members here
}

//resets all the player attributes to their original values
void Player::reset(){
    // *This must be defined in the body of the constructor, since it is
    // inherited from the GameObject class (see the C++ Programming Tips page on
    // the Wiki).
    life = maxLife;

    // Actually, may want to intialize this to the coordinates of the mouse,
    // if this information is available when the screen is first drawn.
    // may need to change this later to take the avatar's size into
    // consideration
    x = GameEngine::MAX_X / 2;
    y = GameEngine::MAX_Y / 2;

    // Dummy values for the previous position of the avatar on the last call to
    // update. This will be initialized to the correct value on the first call
    // to update.
    x2 = x;
    y2 = y;

    //set initial score;
    score = 0;
}


// Implementation of pure virtual functions.

void Player::update() {
    // Update the previous coordinates of the avatar with the current
    // coordinates.
    x2 = x;
    y2 = y;

    // Now get the current position of the player's mouse and check it for
    // validity.
    QPoint mousePos(MainWindow::getInstance()->getMousePos());
    if (isValidMousePos(mousePos)){
        // then update the avatar's position with the current mouse coordinates
        x = mousePos.x();
        y = GameEngine::MAX_Y - mousePos.y(); //window uses top-right as 0,0
    }
    // otherwise leave the avatar's position unchanged

    //TESTING (drop a particle every frame when the mouse is pressed)
    //TODO: change this to take into account mouse clicks and distance moved
    //(place a particle every interval on the line between x2,y2 and x,y)
    if (MainWindow::getInstance()->getMouseState() & Qt::LeftButton){
        ObjectManager::getInstance()->spawnParticle(x, y);
    }
    if (MainWindow::getInstance()->getMouseState() & Qt::RightButton){
        //magnitue is really high, take a look at this
        ObjectManager::getInstance()->applyForce(ObjectManager::PARTICLE, x, y, 100000);
    }
}

//draw the player
void Player::draw() const {
    //draw concentric octagons that represent health
    for (int i = MAX_DIAMETER / RING_SIZE * getLifePercent(); i >= 0 ; i--){
        Util::drawOctagon(x, y, 2 + (i + 1) * RING_SIZE, true, ResourceManager::getInstance()->getColourScale(1 - (i/(float)(RING_SIZE+1))));
    }
    //Util::drawTexture(x - size/2.0, y - size/2.0, x + size/2.0, y + size/2.0, ResourceManager::getInstance()->getTexture(ResourceManager::PLAYER));
}

//change the score
void Player::modScore(const int amt, const bool rel){
    if (rel)
        score = score + amt > 0 ? score + amt : 0;
    else
        score = amt > 0 ? amt : 0;
}

// Implementation of private member functions.

void Player::modMana(int amt, bool rel) {
    // Perform bounds checking on what the new value of mana would be.
    if (rel)
        mana = std::max(0, std::min (mana + amt, MAX_MANA));
    else
        mana = std::max(0, std::min (amt, MAX_MANA));
}

bool Player::isValidMousePos(const QPoint& pos) {
    int mouseX(pos.x()), mouseY(pos.y());
    return (mouseX >= 0 && mouseX <= GameEngine::MAX_X) &&
            (mouseY >= 0 && mouseY <= GameEngine::MAX_Y);
}

void Player::useAbility() const {

}

void Player::changeAbility() {

}
