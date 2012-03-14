#include "gameengine.h"
#include "mainwindow.h"
#include "util.h"
#include <time.h>

//ick, redundancy
const int GameEngine::MAX_X;
const int GameEngine::MAX_Y;
const int GameEngine::MAX_FPS;
const int GameEngine::FPS_COUNT_FRAME_INTERVAL;
const int GameEngine::LINES_PER_FADE;
const int GameEngine::FADE_BORDER_AMT;
const int GameEngine::GAME_OVER_FRAMES;
const float GameEngine::FORCE_DISSIPATION;

GameEngine::GameEngine(QWidget *parent) : QGLWidget(parent){
    setFixedSize(MAX_X, MAX_Y);
    setWindowTitle("Particle Storm");
    setAutoFillBackground(false);

    //pseudo-randomness
    qsrand((unsigned int)time(NULL));    

    //FPS timer
    timer = new QTime();

    //Object manager
    objectManager = new ObjectManager();

    //initial garbage value for gameClock
    gameClock = 0;
}

//destructor
GameEngine::~GameEngine(){
    //framebuffer
    delete fbo;

    //managers
    delete objectManager;
    delete resourceManager;

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

    //init the testing values
    for (int i = 0 ; i < 6 ; i++){
        coords[i] = qrand() % (i % 2 == 0 ? MAX_X : MAX_Y);
        vels[i] = (qrand() % 5) + 5;
    }

    //game states
    paused = false;
    gameOverFrames = 0;

    //FPS stuff
    framecnt = 0;
    fps = 0;
    timer->start();

    //tell the framebuffer to clear properly
    initialClear = true;

    //reset mouse state
    mouseState = Qt::NoButton;

    //start the timer (sets off the frameloop via timerEvent)
    gameClock = startTimer(1/(double)MAX_FPS*1000);
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
        Util::drawBox(0, 0, MAX_X, FADE_BORDER_AMT, true);
        Util::drawBox(0, MAX_Y - FADE_BORDER_AMT, MAX_X - FADE_BORDER_AMT, MAX_Y, true);
        Util::drawBox(0, FADE_BORDER_AMT, FADE_BORDER_AMT, MAX_Y - FADE_BORDER_AMT, true);
        Util::drawBox(MAX_X - FADE_BORDER_AMT, FADE_BORDER_AMT, MAX_X, MAX_Y, true);

        //fade the screen
        for (int i = 0 ; i < LINES_PER_FADE ; i++){
            glBegin(GL_LINES);
                glVertex2i(qrand() % MAX_X, qrand() % MAX_Y);
                glVertex2i(qrand() % MAX_X, qrand() % MAX_Y);
            glEnd();
        }
    }
}

//test scene
void GameEngine::drawScene(){
    //draw lightning every 3 frames
    if (framecnt % 3 == 0)
        Util::drawJaggedLine(qrand() % MAX_X, qrand() % MAX_Y, qrand() % MAX_X, qrand() % MAX_Y, resourceManager->getColour(ResourceManager::WHITE));

    //draw triangles
    glBegin(GL_TRIANGLES);
        glColor3f(1, 0, 0); glVertex2i(coords[0], coords[1]);
        glColor3f(0, 1, 0); glVertex2i(coords[2], coords[3]);
        glColor3f(0, 0, 1); glVertex2i(coords[4], coords[5]);
    glEnd();

    //zap the green corner of the triangle from 100, 100 with green lightning
    Util::drawJaggedLine(100, 100, coords[2], coords[3], resourceManager->getColour(ResourceManager::GREEN));
}

//draws information for the player
void GameEngine::drawHUD(){

    //demo bars for health + mana
    Util::drawMeter(20, MAX_Y - 35, 220, MAX_Y - 20, objectManager->getPlayer()->getLifePercent(), false, resourceManager->getColour(ResourceManager::RED));
    Util::drawMeter(240, MAX_Y - 35, 440, MAX_Y - 20, objectManager->getPlayer()->getManaPercent(), false, resourceManager->getColour(ResourceManager::BLUE));
    //Score text (frames for now)
    Util::drawString("SCORE:", MAX_X - 260, MAX_Y - 25, resourceManager->getTexture(ResourceManager::TEXT), false, true);
    Util::drawString(Util::doubleToString(objectManager->getPlayer()->getScore(), 10, 0), MAX_X - 90, MAX_Y - 25, resourceManager->getTexture(ResourceManager::TEXT), true, true, 1, Util::getScaleByFrame(framecnt, 25, 1, 2));
    //FPS text
    Util::drawString("FPS: " + Util::doubleToString(fps, 4, 1), 0, 0, resourceManager->getTexture(ResourceManager::TEXT));
}

//update game logic - automatically called
void GameEngine::update(){
    //FPS monitoring
    framecnt++;
    if (framecnt % FPS_COUNT_FRAME_INTERVAL == 0){
        fps = FPS_COUNT_FRAME_INTERVAL/(double)(timer->restart()) * 1000;
    }

    //game over
    if (gameOverFrames > 0 || objectManager->getPlayer()->getLife() == 0){
        gameOverFrames++;
        if (gameOverFrames > GAME_OVER_FRAMES){
            paused = true;
            MainWindow::getInstance()->doneGame(objectManager->getPlayer()->getScore());
        }
        //don't bother updating anything else
        return;
    }

    //bouncing triangle - basic bounds
    for (int i = 0 ; i < 6 ; i++){
        if (coords[i] > (i % 2 == 0 ? MAX_X : MAX_Y)){
            vels[i] *= -1;
            coords[i] = (i % 2 == 0 ? MAX_X : MAX_Y);
        }
        else if (coords[i] < 0){
            vels[i] *= -1;
            coords[i] = 0;
        }
        //apply velocity
        coords[i] += vels[i];
    }

    //game stuff
    objectManager->update(ObjectManager::PLAYER);
    objectManager->update(ObjectManager::PARTICLE);

    //testing game stuff
    objectManager->modPlayerScore(1);
    //if (getMouseState() & Qt::RightButton){
    //    objectManager->modPlayerLife(-5);
    //}
}

//draws everything - automatically called
void GameEngine::paintGL(){
    if (gameOverFrames == 0){
        //draw the game

        /*Draw method:
          To framebuffer:
            Old framebuffer (don't erase it)
            Fade
            Shrapnel
            Powerups
            Particles
            Player (maybe implement selective fading)
          To screen:
            Current framebuffer
            Stars
            Enemies
            HUD
        */

        //all draw commands draw to the framebuffer
        fbo->bind();
        doFade();
        drawScene();

        objectManager->draw(ObjectManager::PARTICLE);

        fbo->release();

        //all draw commands go to screen

        //draw framebuffer (all previous drawing commands)
        Util::drawTexture(0, 0, MAX_X, MAX_Y, fbo->texture());

        objectManager->draw(ObjectManager::PLAYER);

        drawHUD();
    }
    else{
        //draw the game over sequence

        //fade screen (or other game-overy stuff) for GAME_OVER_FRAMES frames
        Util::drawString("Game Over", MAX_X/2, MAX_Y/2, resourceManager->getTexture(ResourceManager::TEXT), true, true, 5, 5);
    }
}

