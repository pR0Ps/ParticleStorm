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
const double Player::COLLISON_INDICATION_TIME = 0.1;
const int Player::MANA_REGEN_DELAY = 5;
const int Player::MANA_REGEN_RATE = 50;

const double Player::TIME_BETWEEN_CHG_ABILITY = 0.5;
const int Player::LIGHTNING_RANGE = 350;
const int Player::LIGHTNING_DPS = 150;
const int Player::LIGHTNING_MANA_COST = 75;
const int Player::MIN_LIGHTNING_DRAW_DISTANCE = 10;
const int Player::SPRAY_PPS = 100;
const int Player::SPRAY_MANA_COST = 50;
const double Player::LIGHTNING_HEAL_MODIFIER = 0.5;
const int Player::SPRAY_PARTICLE_SPEED = 2000;
const int Player::VORTEX_MANA_COST = 100;
const int Player::VORTEX_PPS = 100;
const int Player::VORTEX_SPAWN_RAD = 100;
const int Player::VORTEX_SPAWN_VELOCITY = 1350;
const int Player::SHOCKWAVE_MANA_COST = 100;
const int Player::SHOCKWAVE_PPS = 100;
const int Player::SHOCKWAVE_INNER_RAD = 60;
const int Player::SHOCKWAVE_DPS = 250;
const int Player::REPULSE_RANGE = 200;
const int Player::REPULSE_DPS = 100;
const int Player::REPULSE_FORCE = 500;
const int Player::REPULSE_MANA_COST = 150;
const double Player::REPULSE_TIME = 0.3;
const int Player::STORM_MANA_COST = Player::LIGHTNING_MANA_COST * 2;
// Assign storm the same range as lightning for now.
const int Player::STORM_RANGE = Player::LIGHTNING_RANGE;
const unsigned int Player::NUM_STORM_TARGETS = 3;
// And the same damage per second.
const int Player::STORM_DPS = Player::LIGHTNING_DPS;

// Implementation of constructor and destructor.

// Some data members of the Player class do not need to initialized:
// - x_vel and y_vel -> the avatar's velocity is determined entirely by the
// player's mouse movements, so these are not needed
// inUse also doesn't matter, player is always in use
Player::Player(): GameObject(){
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

    //set mana
    mana = MAX_MANA;

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

    //set not immune + hit
    collisionBufferTime = 0;
    hitDisplayTime = 0;
    repulseDisplayTime = 0;
    manaRegenTime = 0;

    //set initial score
    score = 0;
    currentAbility = VORTEX;

    // Initialize variables for keeping track of when change ability was last
    // used.
    chgAbilityActivatedOnLastUpdate = false;
    timeSinceLastChgAbility = 0;

    lightningTarget = NULL;
    stormTargets = std::vector<Enemy*>();
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

    //decrease timers
    if(collisionBufferTime > 0)
        collisionBufferTime -= deltaTime;
    if(hitDisplayTime > 0)
        hitDisplayTime -= deltaTime;
    if(repulseDisplayTime > 0)
        repulseDisplayTime -= deltaTime;

    // Attempt to regenerate mana if enough time has passed. This should be
    // called regardless of whether or not the special ability key is pressed on
    // the current update, since mana should regenerate if the player does not
    // have enough mana to perform the ability or no enemies are in range in the
    // case of lightning.
    regenMana(deltaTime);
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
    drawStorm();

    //hit indication (an X)
    if (hitDisplayTime > 0){
        const int angle = qrand() % 359;
        const float size = (1 - getLifePercent()) * 50 + 50;

        glColor3d(100, 100, 100);
        for (int i = -6 ; i <= 6 ; i++){
            Util::drawLine(x + size * Util::sind(angle + i), y + size * Util::cosd(angle + i),
                           x - size * Util::sind(angle + i), y - size * Util::cosd(angle + i));
            Util::drawLine(x + size * Util::sind(angle + 90 + i), y + size * Util::cosd(angle + 90 + i),
                           x - size * Util::sind(angle + 90 + i), y - size * Util::cosd(angle + 90 + i));
        }
    }

    //repulsing enemies
    if (repulseDisplayTime > 0){
        Util::drawRoundShape(x, y, (1 - repulseDisplayTime/REPULSE_TIME) * REPULSE_RANGE, 20, false,
                             ResourceManager::getInstance()->getColour(ResourceManager::LIGHTBLUE));
    }
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

void Player::drawStorm() const {
    // Iterate through the list of enemies targeted by storm and draw lightning
    // to them so long as they are not too close to the player. No lightning
    // will be drawn if this vector is empty.
    for (unsigned int i = 0; i < stormTargets.size(); i++) {
        double enemyX = stormTargets[i]->getX();
        double enemyY = stormTargets[i]->getY();

        if (Util::distance(x, y, enemyX, enemyY) > MIN_LIGHTNING_DRAW_DISTANCE)
            Util::drawJaggedLine(x, y, enemyX, enemyY,
            ResourceManager::getInstance()->getColour(ResourceManager::PURPLE));
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
    case STORM:
        return "STORM";
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
        // Ditto for storm targets.
        stormTargets.clear();
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

//player was set as immune, therefore draw hit effects
void Player::setImmune(){
    //show hit indication
    hitDisplayTime = COLLISON_INDICATION_TIME;
    collisionBufferTime = MAX_COLLISION_BUFFER_TIME;

    //shrapnel from player
    ObjectManager::getInstance()->spawnShrapnel(x, y, 0, 0, 8, 20,
        ResourceManager::getInstance()->getColourScale(1 - getLifePercent() * 1.1));

    //screen shake
    MainWindow::getInstance()->shakeGameScreen(5, 10);
}

void Player::forcePush(ObjectManager* manager) const {
    manager->applyForce(ObjectManager::PARTICLE, x, y, Particle::FORCE_EXERT, Particle::FORCE_CUTOFF, Particle::FORCE_DISSIPATION);
    manager->applyForce(ObjectManager::STAR, x, y, Star::FORCE_EXERT, Star::FORCE_CUTOFF, Star::FORCE_DISSIPATION);
}

void Player::forcePull(ObjectManager* manager) const {
    manager->applyForce(ObjectManager::PARTICLE, x, y, -Particle::FORCE_EXERT, Particle::FORCE_CUTOFF,  Particle::FORCE_DISSIPATION);
    manager->applyForce(ObjectManager::STAR, x, y, -Star::FORCE_EXERT, Star::FORCE_CUTOFF, Star::FORCE_DISSIPATION);
}

void Player::useAbility(double deltaTime, ObjectManager* manager) {
    switch (currentAbility) {
    case VORTEX:
        vortexAbility(deltaTime, manager);
        break;
    case SPRAY:
        sprayAbility(deltaTime, manager);
        break;
    case REPULSE:
        repulseAbility(deltaTime, manager);
        break;
    case LIGHTNING:
        lightningAbility(deltaTime, manager);
        break;
    case STORM:
        stormAbility(deltaTime, manager);
        break;
    case SHOCKWAVE:
        shockwaveAbility(deltaTime, manager);
        break;
    }
}

// Would be more efficient to check to see if the player has enough mana first
// before finding the closest enemy.
// Also, use the lightningTarget attribute instead of the intermediary
// closestEnemey variable (unneeded).
void Player::lightningAbility(double deltaTime, ObjectManager* manager) {
    const double manaCost = deltaTime * LIGHTNING_MANA_COST;
    Enemy* closestEnemy = manager->getClosestEnemy(x, y, 0, LIGHTNING_RANGE);

    if (closestEnemy != NULL && mana >= manaCost) {
        // then an enemy is in range of the lightning ability and the player has
        // enough mana left to perform the ability
        double damage = deltaTime * LIGHTNING_DPS;

        closestEnemy->modLife(-damage);
        modMana(-manaCost);
        resetManaRegenTime();

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
    else {
        // reset the current target of the lightning ability to null so that the
        // lightning effect will not be drawn
        lightningTarget = NULL;

        //try to stop play zap
        // ResourceManager::getInstance()->playZap(1, true);
    }
}

void Player::stormAbility(double deltaTime, ObjectManager* manager) {
    // Clear the list of storm targets so that the lightning effect will not be
    // drawn if the player runs out of mana while using the ability, and so that
    // comparisons will not be made to previous storm targets when inserting
    // enemies into the list.
    // It should be noted here that the clear function, as well as pop_back,
    // actually invokes the destructor of elements removed from the vector.
    // However, this is not the same as explicitly deleting a pointer, and there
    // does not appear to be the risk of replacing enemies in the game with
    // garbage when code like this is used.
    stormTargets.clear();

    // Perform mana check first.
    double manaCost = deltaTime * STORM_MANA_COST;
    if (manaCost <= mana) {
        // Find the closest n enemies to the player, if there are any.
        findStormTargets(manager);

        // Apply mana cost and damage if any targets were in range of the storm.
        if (!stormTargets.empty()) {
            modMana(-manaCost);
            resetManaRegenTime();

            double damage = deltaTime * STORM_DPS;
            for (unsigned int i = 0; i < stormTargets.size(); i++)
                stormTargets[i]->modLife(-damage);
        }
    }
}

void Player::findStormTargets(ObjectManager* manager) {
    // The getObjectsInRange function returns a vector of GameObjects within the
    // specified range that are still in use. Make sure to delete this vector
    // when done with it.
    std::vector<GameObject*>* enemies =
            manager->getObjectsInRange(ObjectManager::ENEMY, x, y, STORM_RANGE);

    // Iterate over the list of current enemies and add them to the list of
    // valid storm targets as applicable. The list of storm targets must be kept
    // in nondecreasing order by player distance to enemy in this process so
    // that the closest enemies are targeted by storm, and not just enemies that
    // happen to be in range.
    for (unsigned int i = 0; i < enemies->size(); i++)
        insertEnemy(dynamic_cast<Enemy*> (enemies->at(i)));

    delete enemies;
}

void Player::insertEnemy(Enemy* enemy) {
    double distance = Util::distance(x, y, enemy->getX(), enemy->getY());

    // An iterator must be used here so that we can use the insert function.
    // As soon as a modification is made to the vector, all iterators become
    // invalidated, so we should immediately break out of the loop after the
    // insertion.
    bool inserted = false;
    std::vector<Enemy*>::iterator target = stormTargets.begin();

    while (!inserted && target != stormTargets.end()) {
        if (distance < Util::distance(x, y, (*target)->getX(),
                                      (*target)->getY())) {
            // then the enemy is closer to the player then the currently
            // examined target
            stormTargets.insert(target, enemy);
            inserted = true;
        }

        target++;
    }

    // If we iterated through the entire list of storm targets without inserting
    // the enemy and the number of targets selected so far has not reached the
    // maximum, then the enemy can be added to the end of the list.
    if (!inserted && stormTargets.size() < NUM_STORM_TARGETS)
        stormTargets.push_back(enemy);

    // If instead the maximum number of storm targets was exceeded by inserting
    // the current enemy, then remove the last enemy in the list.
    if (stormTargets.size() > NUM_STORM_TARGETS)
        stormTargets.pop_back();
}

void Player::sprayAbility(double deltaTime, ObjectManager* manager) {
    // First check to see if the player has enough mana to perform the ability.
    double manaCost = deltaTime * SPRAY_MANA_COST;

    if (manaCost <= mana) {
        modMana(-manaCost);
        resetManaRegenTime();

        // Spawn numParticles at the player's current position and give them an
        // initial velocity. This controls their direction.
        // The ceil function is used so that at least one particle is spawned
        // per update. This may actually cause more particles to spawn per
        // second than is specified by one of the spray ability's constants, but
        // this is a minor issue.
        int numParticles = ceil(deltaTime * SPRAY_PPS);
        for (int particleCount = 0; particleCount < numParticles;
             particleCount++) {
            // Randomly choose a degree of direction for the spawned particle.
            int degree = Util::randInt(0, 360);

            // Use sin to calculate the x velocity and cos for the y velocity.
            // Note: the cmath trigonometric functions take radians instead of
            // degrees - use the Util functions instead.
            double xVel = Util::sind(degree) * SPRAY_PARTICLE_SPEED;
            double yVel = Util::cosd(degree) * SPRAY_PARTICLE_SPEED;
            manager->spawnParticle(x, y, xVel, yVel);
        }
    }
}

void Player::vortexAbility(double deltaTime, ObjectManager* manager) {
    // Check to see if the player has enough mana left to perform the ability.
    double manaCost = deltaTime * VORTEX_MANA_COST;
    if (manaCost <= mana) {
        modMana(-manaCost);
        resetManaRegenTime();

        // Spawn particles around the player. The location of the spawned
        // particles is randomly chosen.
        // The particles spawned are also given an initial velocity
        // perpendicular to the degree used to choose the starting location of
        // the particle. This way, when a pulling force is applied to the
        // particles they will swirl around the player, creating the vortex.
        // (Thanks to Adam/Carey for this.)
        int numParticles = ceil(deltaTime * VORTEX_PPS);
        for (int i = 0; i < numParticles; i++) {
                int degree = Util::randInt(0, 360);
                double xPos = x + Util::sind(degree) * VORTEX_SPAWN_RAD;
                double yPos = y + Util::cosd(degree) * VORTEX_SPAWN_RAD;
                double xVel = Util::sind(degree + 90) * VORTEX_SPAWN_VELOCITY;
                double yVel = Util::cosd(degree + 90) * VORTEX_SPAWN_VELOCITY;

                manager->spawnParticle(xPos, yPos, xVel, yVel);
        }

        // Apply a pulling force to all of the particles just spawned, and all
        // of the particles that were already in the game. This is essentially
        // the same as the force pull ability except that the force is not also
        // applied to stars.
        manager->applyForce(ObjectManager::PARTICLE, x, y, -Particle::FORCE_EXERT, Particle::FORCE_CUTOFF, Particle::FORCE_DISSIPATION);
    }
}

void Player::shockwaveAbility(double deltaTime, ObjectManager *manager){
    //check mana
    const double manaCost = deltaTime * SHOCKWAVE_MANA_COST;
    if (manaCost <= mana){
        modMana(-manaCost);
        resetManaRegenTime();

        //particles are mostly for show
        const int numParticles = ceil(deltaTime * SHOCKWAVE_PPS);
        for (int i = 0; i < numParticles; i++) {
                int degree = Util::randInt(0, 360);
                double xPos = x + Util::sind(degree) * SHOCKWAVE_INNER_RAD;
                double yPos = y + Util::cosd(degree) * SHOCKWAVE_INNER_RAD;
                manager->spawnParticle(xPos, yPos, 0, 0);
        }

        //damage all nearby enemies
        std::vector<GameObject*> *temp = manager->getObjectsInRange(ObjectManager::ENEMY, x, y, SHOCKWAVE_INNER_RAD);
        for (unsigned int i = 0 ; i < temp->size() ; i++){
            temp->at(i)->modLife(-SHOCKWAVE_DPS * deltaTime);
        }
        delete temp;

        //pull particles in, but push them out as well. this creates a bunch of oscilating particles
        manager->applyForce(ObjectManager::PARTICLE, x, y, -Particle::FORCE_EXERT, Particle::FORCE_CUTOFF, Particle::FORCE_DISSIPATION);
        manager->applyForce(ObjectManager::PARTICLE, x, y, Particle::FORCE_EXERT * 10, SHOCKWAVE_INNER_RAD + 10, Particle::FORCE_DISSIPATION);
    }
}

//pretty broken, enemies don't really take force being applied to them well
void Player::repulseAbility(double deltaTime, ObjectManager *manager){
    //check mana
    const double manaCost = deltaTime * REPULSE_MANA_COST;
    if (manaCost <= mana){
        modMana(-manaCost);
        resetManaRegenTime();

        //damage all nearby enemies
        std::vector<GameObject*> *temp = manager->getObjectsInRange(ObjectManager::ENEMY, x, y, REPULSE_RANGE);
        for (unsigned int i = 0 ; i < temp->size() ; i++){
            temp->at(i)->modLife(-REPULSE_DPS * deltaTime);
        }
        delete temp;

        //push them back
        manager->applyForce(ObjectManager::ENEMY, x, y, REPULSE_FORCE, REPULSE_RANGE, Enemy::FORCE_DISSIPATION);

        //show repulse animation
        if(repulseDisplayTime <= 0)
            repulseDisplayTime = REPULSE_TIME;
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

void Player::regenMana(double deltaTime) {
    if (manaRegenTime <= 0)
        modMana(deltaTime * MANA_REGEN_RATE);
    else
        manaRegenTime -= deltaTime;
}
