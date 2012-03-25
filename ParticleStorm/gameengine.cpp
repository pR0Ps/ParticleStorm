#include "gameengine.h"
#include "mainwindow.h"
#include "util.h"
#include <time.h>
#include <QDebug>

//ick, redundancy
const int GameEngine::MAX_X;
const int GameEngine::MAX_Y;
const int GameEngine::MAX_FPS;
const int GameEngine::FPS_COUNT_FRAME_INTERVAL;
const int GameEngine::LINES_PER_FADE;
const int GameEngine::CLEAR_BORDER_AMT;
const double GameEngine::GAME_OVER_SECONDS;
const float GameEngine::PAN_SPEED;

GameEngine::GameEngine(QWidget *parent) : QGLWidget(parent){
    setFixedSize(MAX_X, MAX_Y);
    setWindowTitle("Particle Storm");
    setAutoFillBackground(false);

    //pseudo-randomness
    qsrand((unsigned int)time(NULL));    

    //FPS/deltaTime timer
    timer = new QTime();

    //set up keypress mapping
    keyMap.insert(Qt::Key_A, DROP);
    keyMap.insert(Qt::Key_S, PUSH);
    keyMap.insert(Qt::Key_D, PULL);
    keyMap.insert(Qt::Key_Space, ABILITY);
    keyMap.insert(Qt::Key_Control, CHGABILITY);
    keyMap.insert(Qt::Key_Escape, EXIT);

    //init keypress arrays
    for (int i = 0 ; i < EXIT+1 ; i++){
        currKeys[i] = false;
    }

    //initially paused
    paused = true;

    //initial garbage value for gameClock
    gameClock = 0;

    //initial values for the managers
    objectManager = NULL;
    resourceManager = NULL;
}

//destructor
GameEngine::~GameEngine(){
    //framebuffer
    delete fbo;

    //managers
    if (objectManager != NULL) delete objectManager;
    if (resourceManager != NULL) delete resourceManager;

    //timer
    delete timer;
}

//the main frameloop
void GameEngine::timerEvent(QTimerEvent *){
    if (!paused){
        update();
        updateGL();
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
    format.setInternalTextureFormat(GL_RGB);
    format.setTextureTarget(GL_TEXTURE_2D);
    fbo = new QGLFramebufferObject(MAX_X, MAX_Y, format);

    //load textures
    resourceManager = new ResourceManager();

    //init object manager (needs resource manager)
    objectManager = new ObjectManager();
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

//resets the game
void GameEngine::reset(){
    if (gameClock != 0){
        killTimer(gameClock);
    }

    //resets all the game objects
    objectManager->reset();

    qDebug() << "GameEngine reset";
}

//starts a game
void GameEngine::start(){
    //game states
    paused = false;
    gameOverTimer = 0;

    //FPS stuff
    framecnt = 0;
    fps = 0;
    secSinceFrameInterval = 0;
    timer->start();

    //tell the framebuffer to clear properly
    initialClear = true;

    //reset mouse state
    mouseState = Qt::NoButton;

    //start the timer (sets off the frameloop via timerEvent)
    gameClock = startTimer(1/(double)MAX_FPS*1000);

    //spawn some testing enemies
    //objectManager->spawnEnemy(ObjectManager::GRUNT, 100, 100, objectManager->getPlayer()->getX(), objectManager->getPlayer()->getY());
    //objectManager->spawnEnemy(ObjectManager::HEALER, 200, 100, objectManager->getPlayer()->getX(), objectManager->getPlayer()->getY());
    //objectManager->spawnEnemy(ObjectManager::TANK, 300, 100, objectManager->getPlayer()->getX(), objectManager->getPlayer()->getY());
    //objectManager->spawnEnemy(ObjectManager::SPRINTER, 400, 100, objectManager->getPlayer()->getX(), objectManager->getPlayer()->getY());
    objectManager->spawnEnemy(ObjectManager::SHOOTER, 500, 100, objectManager->getPlayer()->getX(), objectManager->getPlayer()->getY());
    objectManager->spawnEnemy(ObjectManager::SHOOTER, 400, 100, objectManager->getPlayer()->getX(), objectManager->getPlayer()->getY());
    objectManager->spawnEnemy(ObjectManager::SHOOTER, 300, 100, objectManager->getPlayer()->getX(), objectManager->getPlayer()->getY());
    //objectManager->spawnEnemy(ObjectManager::BULLET, 600, 100, objectManager->getPlayer()->getX(), objectManager->getPlayer()->getY());

    objectManager->spawnPowerup(ObjectManager::HEALTH, 700, 100, 50, 50, 50);
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
        for (int i = 0 ; i < LINES_PER_FADE ; i++){
            glBegin(GL_LINES);
                glVertex2i(qrand() % MAX_X, qrand() % MAX_Y);
                glVertex2i(qrand() % MAX_X, qrand() % MAX_Y);
            glEnd();
        }
    }
}

//draws information for the player
void GameEngine::drawHUD(){

    //demo bars for health + mana
    Util::drawMeter(20, MAX_Y - 35, 220, MAX_Y - 20, objectManager->getPlayer()->getLifePercent(), false, resourceManager->getColour(ResourceManager::RED));
    Util::drawMeter(240, MAX_Y - 35, 440, MAX_Y - 20, objectManager->getPlayer()->getManaPercent(), false, resourceManager->getColour(ResourceManager::BLUE));

    // Display the player's currently selected ability at the top of the screen.
    Util::drawString("ABILITY: " + objectManager->getPlayer()->getAbilityString(), 480,
        MAX_Y - 25, resourceManager->getTexture(ResourceManager::TEXT), false, true);

    //Score text (frames for now)
    Util::drawString("SCORE:", MAX_X - 260, MAX_Y - 25, resourceManager->getTexture(ResourceManager::TEXT), false, true);
    Util::drawString(Util::doubleToString(objectManager->getPlayer()->getScore(), 10, 0), MAX_X - 90, MAX_Y - 25, resourceManager->getTexture(ResourceManager::TEXT), true, true, 1, Util::getScaleByFrame(framecnt, 25, 1, 2));
    //FPS text
    Util::drawString("FPS: " + Util::doubleToString(fps, 4, 1), 0, 0, resourceManager->getTexture(ResourceManager::TEXT));
}

//update game logic - automatically called by timer
void GameEngine::update(){
    //get time (in seconds) since last update
    double deltaTime = timer->restart()/(float)1000;

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
    //should add a pause option here as well
    if (keyPressed(EXIT)){
        exit(0);
    }

    //pan everything
    panX = (MAX_X / (double)2.0 - objectManager->getPlayer()->getX()) * PAN_SPEED * deltaTime;
    panY = (MAX_Y / (double)2.0 - objectManager->getPlayer()->getY()) * PAN_SPEED * deltaTime;
    objectManager->pan(ObjectManager::PARTICLE, panX, panY);
    objectManager->pan(ObjectManager::ENEMY, panX, panY);
    objectManager->pan(ObjectManager::STAR, panX, panY);
    objectManager->pan(ObjectManager::POWERUP, panX, panY);
    objectManager->pan(ObjectManager::SHRAPNEL, panX, panY);

    //update everything
    objectManager->update(ObjectManager::PLAYER, deltaTime);
    objectManager->update(ObjectManager::PARTICLE, deltaTime);
    objectManager->update(ObjectManager::ENEMY, deltaTime);
    objectManager->update(ObjectManager::STAR, deltaTime);
    objectManager->update(ObjectManager::POWERUP, deltaTime);
    objectManager->update(ObjectManager::SHRAPNEL, deltaTime);

    //do collisions after everything has updated into place
    objectManager->doEnemyParticleCollisions();
    objectManager->doPlayerEnemyCollisions();
    //objectManager->doPlayerPowerupCollisions();

    //testing game stuff
    objectManager->modPlayerScore(1);
    /*if (getMouseState() & Qt::RightButton){
        objectManager->modPlayerLife(-5);
    }*/
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
        Util::drawTexture(0 + panX, 0 + panY, MAX_X + panX, MAX_Y + panY, fbo->texture());

        //fade the previous frame
        doFade();

        //draw blurred objects
        objectManager->draw(ObjectManager::SHRAPNEL);
        objectManager->draw(ObjectManager::POWERUP);
        objectManager->draw(ObjectManager::PARTICLE);
        if (getMouseState() & Qt::LeftButton || keyPressed(GameEngine::DROP)){
            objectManager->draw(ObjectManager::PLAYER);
        }

        fbo->release();
        //all draw commands after here go to screen

        //draw current framebuffer
        Util::drawTexture(0, 0, MAX_X, MAX_Y, fbo->texture());

        //draw objects that shouldn't be faded
        objectManager->draw(ObjectManager::STAR);
        objectManager->draw(ObjectManager::ENEMY);
        objectManager->draw(ObjectManager::PLAYER);

        //draw the interface and information
        drawHUD();
    }
    else{
        //draw the game over sequence

        //fade screen (or other game-overy stuff) for GAME_OVER_FRAMES frames
        Util::drawString("Game Over", MAX_X/2, MAX_Y/2, resourceManager->getTexture(ResourceManager::TEXT), true, true, 5, 5);
    }
}

void spawnPosition(){
    /*
    int start = qrand()%4;
    if(start == 0){
        x_start = 0;
        y_start = qrand()%768;
    }
    else if(start == 1){
        x_start = 1024;
        y_start = qrand()%768;
    }
    else if(start == 2){
        x_start = qrand()%1024;
        y_start = 0;
    }
    else if(start == 3){
        x_start = qrand()%1024;
        y_start = 768;
    }*/
}
