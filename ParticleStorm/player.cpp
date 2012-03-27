/**
 * Implementation file for the Player class.
 *
 * Last modified by: Luke
 * Last modified on: March 25, 2012
 *
 * To do:
 * - make the player invulnerable when it takes damage
 * - spawn shrapnel when the player takes damage
 */

#include "player.h"
#include "gameengine.h"
#include "mainwindow.h"
#include "resourcemanager.h"


// Initialization of constants. Some of these can be modified to alter the
// difficulty of the gameplay.
const int Player::MAX_LIFE = 1000;
const int Player::MAX_MANA = 1000;
const int Player::MAX_DIAMETER = 24;
const int Player::RING_SIZE = 4;
const int Player::PARTICLE_SPACING = 15;
const int Player::RAM_DAMAGE = 10;
const double Player::MAX_COLLISION_BUFFER_TIME = 0.8;
const double Player::TIME_BETWEEN_CHG_ABILITY = 0.5;
const int Player::LIGHTNING_RANGE = 350;
const int Player::LIGHTNING_DPS = 150;
const int Player::LIGHTNING_MANA_COST = 50;
const int Player::MIN_LIGHTNING_DRAW_DISTANCE = 10;
const int Player::PARTICLES_SPAWNED_PER_SEC = 200;
const int Player::SPRAY_MANA_COST = 50;
const double Player::LIGHTNING_HEAL_MODIFIER = 0.5;
const int Player::SPRAY_PARTICLE_SPEED = 2000;

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

//resets all the player attributes to their original values - this is called at
//the end of the constructor
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
    x_old = x;
    y_old = y;

    //set not immue
    collisionBufferTime = 0;

    //set initial score
    score = 0;
    currentAbility = VORTEX;

    // Initialize variables for keeping track of when change ability was last
    // used.
    chgAbilityActivatedOnLastUpdate = false;
    timeSinceLastChgAbility = 0;

    lightningTarget = NULL;
}


// Implementation of pure virtual functions.



void Player::update(double deltaTime) {

    // Update the previous coordinates of the avatar with the current
    // coordinates.
    x_old = x;
    y_old = y;

    //repeatedly needed, get temp pointers
    ObjectManager* o = ObjectManager::getInstance();
    MainWindow* mw = MainWindow::getInstance();

    // Now get the current position of the player's mouse and check it for
    // validity.
    QPoint mousePos(mw->getMousePos());
    if (isValidMousePos(mousePos)){
        // then update the avatar's position with the current mouse coordinates
        x = mousePos.x();
        y = mousePos.y();
    }
    // otherwise leave the avatar's position unchanged

    // Perform an ability if any have been activated.
    performAbility(deltaTime, o, mw);

    //decrease collision buffer time
    if(collisionBufferTime > 0)
        collisionBufferTime -= deltaTime;
}

//draw the player - these are called in the GameEngine
void Player::drawNoFade() const {
    if (!(MainWindow::getInstance()->getMouseState() & Qt::LeftButton)){
        drawPlayer();
    }
}

void Player::drawFaded() const {
    if (MainWindow::getInstance()->getMouseState() & Qt::LeftButton){
        drawPlayer();
    }
    drawLightning();
}

void Player::drawPlayer() const {
    //draw concentric octagons that represent health
    for (int i = MAX_DIAMETER / RING_SIZE * getLifePercent(); i >= 0 ; i--){
        Util::drawRoundShape(x, y, 2 + (i + 1) * RING_SIZE, 8, true, ResourceManager::getInstance()->getColourScale(1 - (i/(float)(RING_SIZE+1))));
    }
    Util::drawLine(x, y, x_old, y_old, ResourceManager::getInstance()->getColour(ResourceManager::BLUE));
}

void Player::drawLightning() const {
    // If a target for the lightning ability has been selected then the ability
    // was activated in the current frame.
    if (lightningTarget != NULL) {
        // Perform bounds checking on the distance to the enemy.
        double enemyX = lightningTarget->getX();
        double enemyY = lightningTarget->getY();
        double enemyDistance = Util::distance(x, y, enemyX, enemyY);

        if (enemyDistance > MIN_LIGHTNING_DRAW_DISTANCE) {
            // then the lightning effect can be safely drawn

            // Get a reference to the ResourceManager since this contains the
            // colour that we want for drawing the effect.
            ResourceManager* manager = ResourceManager::getInstance();
            Util::drawJaggedLine(x, y, enemyX, enemyY,
                                 manager->getColour(ResourceManager::YELLOW));
        }
    }
}

//change the score
// Note: there is no upper bound on the player's score, unlike the player's
// mana.
void Player::modScore(const int amt, const bool rel){
    if (rel)
        score = score + amt > 0 ? score + amt : 0;
    else
        score = amt > 0 ? amt : 0;
}

void Player::modMana(double amount, bool rel) {
    //dont decreate mana if in zen mode
    if (MainWindow::getInstance()->getGameType() == LevelManager::ZEN) return;

    // Perform bounds checking on what the new value of mana would be.
    if (rel)
        mana = Util::max(0, Util::min (mana + amount, MAX_MANA));
    else
        mana = Util::max(0, Util::min (amount, MAX_MANA));
}

std::string Player::getAbilityString() const {
    switch (currentAbility) {
    case VORTEX:
        return "VORTEX";
    case SPRAY:
        return "SPRAY";
    case REPULSE:
        return "REPULSE";
    case LIGHTNING:
        return "LIGHTNING";
    case SHOCKWAVE:
        return "SHOCKWAVE";
    }

    return "NONE"; // dummy return statement to remove compilation warning
}

// Implementation of private member functions.

bool Player::isValidMousePos(const QPoint& pos) {
    int mouseX(pos.x()), mouseY(pos.y());
    return (mouseX >= 0 && mouseX <= GameEngine::MAX_X) &&
            (mouseY >= 0 && mouseY <= GameEngine::MAX_Y);
}

void Player::performAbility(double deltaTime, ObjectManager* manager,
                            MainWindow* window) {
    // Check to see if the drop particles ability has been activated.
    if ((window->getMouseState() & Qt::LeftButton) ||
            window->getKeyPressed(GameEngine::DROP))
        dropParticles(manager);
    // Force push ability. Force push and force pull are only applied to
    // particles and stars.
    else if ((window->getMouseState() & Qt::RightButton) ||
             window->getKeyPressed(GameEngine::PUSH))
        forcePush(manager);
    // Force pull.
    else if ((window->getMouseState() & Qt::MiddleButton) ||
             window->getKeyPressed(GameEngine::PULL))
        forcePull(manager);
    // Use special ability.
    else if (window->getKeyPressed(GameEngine::ABILITY))
        useAbility(deltaTime, manager);
    // Change special ability.
    else if (window->getKeyPressed(GameEngine::CHGABILITY))
        changeAbility(deltaTime);

    // Perform any clean-up required for when a special ability has not been
    // activated.
    if (!window->getKeyPressed(GameEngine::ABILITY)) {
        // Reset the lightning target to prevent the lightning effect from
        // continuing to be drawn if this ability was activated on the last call
        // to update.
        lightningTarget = NULL;
    }
    // If the change ability button is not pressed, then reset the variables for
    // keeping track of how often the player's special ability can be changed.
    if (!window->getKeyPressed(GameEngine::CHGABILITY)) {
        chgAbilityActivatedOnLastUpdate = false;
        timeSinceLastChgAbility = 0;
    }
}

void Player::dropParticles(ObjectManager* manager) const {
    // spwan at least one particle (if staying still)
    manager->spawnParticle(x, y);

    // spawn a particle every PARTICLE_SPACING px along the line between the
    // old a new pos
    // Note: may want to use the time since the last update for this
    // instead.
    const double dist = Util::distance(x, y, x_old, y_old);
    const double tempX = (x - x_old) / dist;
    const double tempY = (y - y_old) / dist;
    for (int i = 0 ; i < dist ; i+= PARTICLE_SPACING)
        manager->spawnParticle(x_old + tempX * i, y_old + tempY * i);

    /*
    // deltaTime based implementation of drop particles.
    int numParticles = ceil(deltaTime * PARTICLES_DROPPED_PER_SEC);

    // If only one particle is to be spawned then spawn it halfway between the
    // player's current position and their previous position.
    if (numParticles == 1)
        manager->spawnParticle((x + x_old) / 2, (y + y_old) / 2);
    // Otherwise spawn multiple particles along the player's line of movement by
    // using incremental steps for the x and y coordinates.
    else {
        double xStep = (x - x_old) / (numParticles - 1);
        double yStep = (y - y_old) / (numParticles - 1);
        for (int particleCount = 0; particleCount < numParticles;
             particleCount++)
            manager->spawnParticle(x_old + xStep * particleCount,
                                   y_old + yStep * particleCount);
    }
    */
}

void Player::forcePush(ObjectManager* manager) const {
    manager->applyForce(ObjectManager::PARTICLE, x, y, Particle::FORCE_EXERT);
    manager->applyForce(ObjectManager::STAR, x, y, Star::FORCE_EXERT);
}

void Player::forcePull(ObjectManager* manager) const {
    manager->applyForce(ObjectManager::PARTICLE, x, y, -Particle::FORCE_EXERT);
    manager->applyForce(ObjectManager::STAR, x, y, -Star::FORCE_EXERT);
}

void Player::useAbility(double deltaTime, ObjectManager* manager) {
    switch (currentAbility) {
    case VORTEX:
        break;
    case SPRAY:
        sprayAbility(deltaTime, manager);
        break;
    case REPULSE:
        break;
    case LIGHTNING:
        lightningAbility(deltaTime, manager);
        break;
    case SHOCKWAVE:
        break;
    }
}

// Would be more efficient to check to see if the player has enough mana first
// before finding the closest enemy.
void Player::lightningAbility(double deltaTime, ObjectManager* manager) {
    Enemy* closestEnemy = manager->getClosestEnemy(x, y, 0, LIGHTNING_RANGE);
    double manaCost = deltaTime * LIGHTNING_MANA_COST;

    if (closestEnemy != NULL && manaCost <= mana) {
        // then an enemy is in range of the lightning ability and the player has
        // enough mana left to perform the ability
        double damage = deltaTime * LIGHTNING_DPS;

        closestEnemy->modLife(-damage);
        modMana(-manaCost);

        // Heal the player for a percentage of the damage dealt to the target.
        // If the damage of the lightning ability was greater than the enemy's
        // remaining life then heal the player for a fraction of the enemy's
        // remaining life, not the damage that would have been dealt.
        double damageDealt = Util::min(damage, closestEnemy->getLife());
        modLife(damageDealt * LIGHTNING_HEAL_MODIFIER);

        lightningTarget = closestEnemy;
        //try and play zap and stop current music[technically not working but still sounds cool]
        //ResourceManager::getInstance()->playMainMusic(1, true);
       // ResourceManager::getInstance()->playZap(deltaTime, false);

    }
    else{
        // reset the current target of the lightning ability to null so that the
        // lightning effect will not be drawn
        lightningTarget = NULL;
        //try to stop play zap
        // ResourceManager::getInstance()->playZap(1, true);
        }
    }

void Player::sprayAbility(double deltaTime, ObjectManager *manager) {
    // First check to see if the player has enough mana to perform the ability.
    double manaCost = deltaTime * SPRAY_MANA_COST;

    if (manaCost <= mana) {
        modMana(-manaCost);

        // Spawn numParticles at the player's current position and give them an
        // initial velocity. This controls their direction.
        // The ceil function is used so that at least one particle is spawned
        // per update. This may actually cause more particles to spawn per
        // second than is specified by one of the spray ability's constants, but
        // this is a minor issue.
        int numParticles = ceil(deltaTime * PARTICLES_SPAWNED_PER_SEC);
        for (int particleCount = 0; particleCount < numParticles;
             particleCount++) {
            // Randomly choose a degree of direction for the spawned particle.
            int degree = Util::randInt(0, 360);

            // Use sin to calculate the x direction and cos for the y direction.
            // Note: the cmath trigonometric functions take radians instead of
            // degrees - use the Util functions instead.
            double xDirection = Util::sind(degree);
            double yDirection = Util::cosd(degree);
            manager->spawnParticle(x, y, xDirection * SPRAY_PARTICLE_SPEED,
                                   yDirection * SPRAY_PARTICLE_SPEED);
        }
    }
}

void Player::changeAbility(double deltaTime) {
    // If the change ability button was not pressed on the last update or the
    // button has been held down long enough to change the ability again then go
    // ahead and change it.
    if (!chgAbilityActivatedOnLastUpdate ||
            timeSinceLastChgAbility > TIME_BETWEEN_CHG_ABILITY) {
        if (currentAbility == SHOCKWAVE)
            // then wraparound back to vortex
            currentAbility = VORTEX;
        else
            currentAbility++;

        timeSinceLastChgAbility = 0;
    }
    else
        timeSinceLastChgAbility += deltaTime;

    chgAbilityActivatedOnLastUpdate = true;
}
