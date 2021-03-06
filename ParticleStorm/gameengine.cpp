#include "gameengine.h"
#include "mainwindow.h"
#include "levelmanager.h"
#include "util.h"
#include <time.h>
#include <QDebug>

//ick, redundancy
const int GameEngine::MAX_X = 1024;
const int GameEngine::MAX_Y = 680;
const int GameEngine::MAX_FPS = 120;
const int GameEngine::FPS_COUNT_FRAME_INTERVAL = 50;
const int GameEngine::FADE_LINES_PER_SECOND = 30000;
const int GameEngine::CLEAR_BORDER_AMT = 5;
const double GameEngine::GAME_OVER_SECONDS = 5;
const float GameEngine::PAN_SPEED = 0.5;
const double GameEngine::RESUME_GAME_LAG = 2;

GameEngine::GameEngine(QWidget *parent) : QOpenGLWidget(parent){
    setWindowTitle("Particle Storm");
    setFixedSize(MAX_X, MAX_Y);
    setAutoFillBackground(false);
    setMouseTracking(true);

    QRect frect = frameGeometry();
    frect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(frect.topLeft());

    //pseudo-randomness
    qsrand((unsigned int)time(NULL));    

    //FPS/deltaTime timer
    timer = new QTime();

    //set up keypress mapping
    keyMap.insert(Qt::Key_A, DROP);
    keyMap.insert(Qt::Key_S, PUSH);
    keyMap.insert(Qt::Key_D, PULL);
    keyMap.insert(Qt::Key_Space, ABILITY);
    keyMap.insert(Qt::Key_Shift, CHGABILITY);
    keyMap.insert(Qt::Key_Escape, EXIT);

    //initially paused
    paused = true;

    //initial garbage value for gameClock
    gameClock = 0;

    //initial values for the objects
    fbo = NULL;
    objectManager = NULL;
    resourceManager = NULL;
    levelManager = NULL;
}

//destructor
GameEngine::~GameEngine(){
    //framebuffer
    if (fbo != NULL) delete fbo;

    //managers
    if (objectManager != NULL) delete objectManager;
    if (resourceManager != NULL) delete resourceManager;
    if (levelManager != NULL) delete levelManager;

    //timer
    delete timer;
}

//the main frameloop
void GameEngine::timerEvent(QTimerEvent *){
    if (!paused){
        //get time (in seconds) since last update
        double deltaTime = timer->restart()/(float)1000;

        //wait for player to get bearings after a resume
        if (resumeTimer > 0){
            resumeTimer -= deltaTime;
            return;
        }

        step(deltaTime);
    }
}

void GameEngine::initializeGL(){
    //get current OpenGL context
    makeCurrent();

    //current version of OpenGL
    qDebug() << "Current OpenGL version:" << (char*)glGetString(GL_VERSION);

    //window stuff
    glViewport(0, 0, MAX_X, MAX_Y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, MAX_X, 0, MAX_Y); //orthogonal, not perspective
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(0, 0, 0, 255);

    //unneeded OpenGL features (will renable on a per-use basis)
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_DITHER);
    glDisable(GL_FOG);
    glDisable(GL_LIGHTING);
    glDisable(GL_LINE_SMOOTH); //could enable, but performance-heavy and not supported for all
    glDisable(GL_POINT_SMOOTH); //same for this
    glDisable(GL_LINE_STIPPLE);
    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_STENCIL_TEST);
    //for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //init the framebuffer
    QGLFramebufferObjectFormat format;
    format.setAttachment(QGLFramebufferObject::NoAttachment);
    format.setInternalTextureFormat(GL_RGBA);
    format.setTextureTarget(GL_TEXTURE_2D);
    fbo = new QGLFramebufferObject(MAX_X, MAX_Y, format);

    //load textures
    resourceManager = new ResourceManager();

    //init object manager (needs resource manager)
    objectManager = new ObjectManager();
    levelManager = new LevelManager();
}

//keyboard stuff
void GameEngine::keyPressEvent(QKeyEvent * event){
    if (keyMap.contains(event->key()))
        currKeys[keyMap.value(event->key())] = true;
}
void GameEngine::keyReleaseEvent(QKeyEvent * event){
    if (keyMap.contains(event->key()))
        currKeys[keyMap.value(event->key())] = false;
}

// Convert the mouse position fetched from Qt to the coordinates that are used
// by OpenGL. For Qt, a y coordinate of 0 is the top of the screen, but for
// OpenGL this is the bottom of the screen.
QPoint GameEngine::getMousePos() const {
    QPoint mousePos = mapFromGlobal(QCursor::pos());
    return QPoint(mousePos.x(), MAX_Y - mousePos.y());
}

//resets the game
void GameEngine::reset(){
    if (gameClock != 0){
        killTimer(gameClock);
    }

    //resets all the game objects
    objectManager->reset();
}

//starts a game
void GameEngine::start(int mode){
    //game states
    paused = false;
    gameOverTimer = 0;
    resumeTimer = 0;

    //set game type
    if (mode < 0 || mode > 2) mode = 0;
    gameMode = (LevelManager::LevelType)mode;

    //game over text
    gameOverText = "GAME OVER";

    //FPS stuff
    framecnt = 0;
    fps = 0;
    secSinceFrameInterval = 0;
    timer->start();

    //init keypress arrays
    for (int i = 0 ; i < EXIT+1 ; i++){
        currKeys[i] = false;
    }

    //don't initially shake the screen
    shakeX = 0;
    shakeY = 0;
    shakeFactor = 0;

    //tell the framebuffer to clear properly
    initialClear = true;

    //initilize the fade count
    fadeCount = 0;

    //reset mouse state
    mouseState = Qt::NoButton;

    //start the timer (sets off the frameloop via timerEvent)
    gameClock = startTimer(1/(double)MAX_FPS*1000);

    //start the game
    levelManager->startLevel((LevelManager::LevelType)gameMode, 1);
}

//pauses the game
void GameEngine::resume(){
    this->paused = false;
    //restart the timer
    timer->start();

    //init keypress arrays
    for (int i = 0 ; i < EXIT+1 ; i++){
        currKeys[i] = false;
    }

    //wait a bit
    resumeTimer = RESUME_GAME_LAG;
}

//shake the screen
void GameEngine::shakeScreen(double time, int amt){
    //not working yet
    //shakeFactor = amt;
    //shakeDissipation = amt/time;
}

//fade the frame
void GameEngine::doFade(){
    glColor3f(0, 0, 0);

    if (initialClear){
        //fully clears the screen
        glClear(GL_COLOR_BUFFER_BIT);
        initialClear = false;
    }
    else{
        //outsides don't get cleared very well
        Util::drawBox(0, 0, MAX_X, CLEAR_BORDER_AMT, true);
        Util::drawBox(0, MAX_Y - CLEAR_BORDER_AMT, MAX_X - CLEAR_BORDER_AMT, MAX_Y, true);
        Util::drawBox(0, CLEAR_BORDER_AMT, CLEAR_BORDER_AMT, MAX_Y - CLEAR_BORDER_AMT, true);
        Util::drawBox(MAX_X - CLEAR_BORDER_AMT, CLEAR_BORDER_AMT, MAX_X, MAX_Y, true);

        //fade the screen
        for (int i = 0 ; i < fadeCount; i++){
            glBegin(GL_LINES);
                glVertex2i(qrand() % MAX_X, qrand() % MAX_Y);
                glVertex2i(qrand() % MAX_X, qrand() % MAX_Y);
            glEnd();
        }
    }
    fadeCount = 0;
}

//draws information for the player
void GameEngine::drawHUD(){

    if (gameMode != LevelManager::ZEN){
        //bars for health + mana
        Util::drawMeter(20, MAX_Y - 32, 220, MAX_Y - 19, objectManager->getPlayer()->getLifePercent(), false, resourceManager->getColour(ResourceManager::RED));
        Util::drawMeter(240, MAX_Y - 32, 440, MAX_Y - 19, objectManager->getPlayer()->getManaPercent(), false, resourceManager->getColour(ResourceManager::BLUE));

        //level text
        Util::drawString(levelManager->getLevelString(), MAX_X - 450, MAX_Y - 25, resourceManager->getTexture(ResourceManager::TEXT), false, true);

        //Score text
        Util::drawString("SCORE:", MAX_X - 260, MAX_Y - 25, resourceManager->getTexture(ResourceManager::TEXT), false, true);
        Util::drawString(Util::doubleToString(objectManager->getPlayer()->getScore(), 10, 0), MAX_X - 90, MAX_Y - 25, resourceManager->getTexture(ResourceManager::TEXT), true, true, 1, Util::getScaleByFrame(framecnt, 25, 1, 2));
    }

    // Display the player's currently selected ability at the top of the screen.
    Util::drawString("ABILITY: " + objectManager->getPlayer()->getAbilityString(), MAX_X - 270,
        0, resourceManager->getTexture(ResourceManager::TEXT));

    //FPS text
    Util::drawString("FPS: " + Util::doubleToString(fps, 4, 1), 0, 0, resourceManager->getTexture(ResourceManager::TEXT));
}

//update game logic - automatically called by timer
void GameEngine::step(double deltaTime){

    //FPS monitoring
    framecnt++;
    secSinceFrameInterval += deltaTime;
    if (framecnt % FPS_COUNT_FRAME_INTERVAL == 0){
        fps = FPS_COUNT_FRAME_INTERVAL/(double)secSinceFrameInterval;
        secSinceFrameInterval = 0;
    }

    //game over
    if (gameOverTimer > 0 || objectManager->getPlayer()->getLife() == 0){
        gameOverTimer += deltaTime;
        if (gameOverTimer > GAME_OVER_SECONDS){
            paused = true;
            MainWindow::getInstance()->doneGame(objectManager->getPlayer()->getScore());
        }
        //compute game over stuff

        return; //don't bother updating anything else
    }

    //game stuff

    //deal with keyboard input
    if (getKeyPressed(EXIT)){
        paused = true;
        MainWindow::getInstance()->pauseGame();
    }

    //deal with shaking
    const int shkAngle = qrand() % 359 + 1;
    shakeX = (shakeFactor * Util::sind (shkAngle));
    shakeY = (shakeFactor * Util::cosd (shkAngle));
    shakeFactor -= (shakeDissipation * deltaTime);
    if (shakeFactor < 1){
        shakeFactor = 0;
        shakeX = shakeY = 0;
    }

    //pan everything
    panX = (MAX_X / (double)2.0 - objectManager->getPlayer()->getX()) * PAN_SPEED * deltaTime + shakeX;
    panY = (MAX_Y / (double)2.0 - objectManager->getPlayer()->getY()) * PAN_SPEED * deltaTime + shakeY;
    objectManager->pan(ObjectManager::PARTICLE, panX, panY);
    objectManager->pan(ObjectManager::ENEMY, panX, panY);
    objectManager->pan(ObjectManager::STAR, panX, panY);
    objectManager->pan(ObjectManager::POWERUP, panX, panY);
    objectManager->pan(ObjectManager::SHRAPNEL, panX, panY);

    //update everything
    objectManager->step(ObjectManager::PLAYER, deltaTime);
    objectManager->step(ObjectManager::PARTICLE, deltaTime);
    objectManager->step(ObjectManager::ENEMY, deltaTime);
    objectManager->step(ObjectManager::STAR, deltaTime);
    objectManager->step(ObjectManager::POWERUP, deltaTime);
    objectManager->step(ObjectManager::SHRAPNEL, deltaTime);

    //do collisions after everything has updated into place
    objectManager->doEnemyParticleCollisions();
    objectManager->doPlayerEnemyCollisions();
    objectManager->doPlayerPowerupCollisions();

    //level stuff
    levelManager->step(deltaTime);
    if (levelManager->levelFinished()){
        if (levelManager->getCurrentLevel() == LevelManager::MAX_LEVEL){
            gameOverText = "YOU WIN!";
            gameOverTimer += deltaTime;
        }
        else levelManager->nextLevel();
    }

    //fading the screen
    fadeCount += FADE_LINES_PER_SECOND * deltaTime;

    //Update OpenGL
    update();
}

//draws everything - automatically called by timer
void GameEngine::paintGL(){
    if (gameOverTimer == 0){
        //draw the game

        /*Draw method:
          To framebuffer:
            Old framebuffer (pan it)
            Fade
            Shrapnel
            Powerups
            Particles
            Player (selective fading when dropping particles)
          To screen:
            Current framebuffer
            Stars
            Enemies
            Player
            HUD
        */

        //all draw commands after here draw to the framebuffer
        fbo->bind();

        //shift framebuffer by pan amount (by offsetting it and drawing it to itself)
        Util::drawTexture(panX, panY, MAX_X + panX, MAX_Y + panY, fbo->texture());

        //fade the previous frame
        doFade();

        //draw blurred objects
        objectManager->drawAll(true);

        fbo->release();
        //all draw commands after here go to screen

        //draw current framebuffer
        Util::drawTexture(0, 0, MAX_X, MAX_Y, fbo->texture());

        //draw objects that shouldn't be faded
        objectManager->drawAll(false);

        //draw the interface and information
        drawHUD();

        //draw the level number display
        levelManager->drawNoFade();
    }
    else{
        //draw the game over sequence

        //fade screen (or other game-overy stuff) for GAME_OVER_FRAMES frames
        Util::drawString(gameOverText, MAX_X/2, MAX_Y/2, resourceManager->getTexture(ResourceManager::TEXT), true, true, 5, 5);
    }
}
