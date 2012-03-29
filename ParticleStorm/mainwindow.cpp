#include "mainwindow.h"
#include "gameengine.h"
#include "util.h"
#include <QSound>
#include <QDir>
#include <iostream>

const int MainWindow::MAX_X;
const int MainWindow::MAX_Y;
const int MainWindow::MAX_DIST;
const int MainWindow::NUM_STARS;
const int MainWindow::STAR_PAN_X;
const int MainWindow::STAR_PAN_Y;
const int MainWindow::CURSOR_OFFSET;

MainWindow* MainWindow::instance = NULL;

MainWindow::MainWindow(QWidget *parent) : QGLWidget(parent){
    setWindowTitle("Particle Storm");
    setFixedSize(MAX_X,MAX_Y);

    QRect frect = frameGeometry();
    frect.moveCenter(QDesktopWidget().availableGeometry().center());
    move(frect.topLeft());

    //set up the game engine
    engine = new GameEngine();
    engine->setMouseTracking(true);

    //self reference
    instance = this;

    gameType = 0;

    muted = false;

    //init stars
    initStars();

    //Buttons relative to texture centers
    levelButton = new Button(145, 300, 440, 330);
    endlessButton = new Button(110, 200, 470, 230);
    zenButton = new Button(170, 100, 410, 130);
    exitButton = new Button(6, 6, 126, 36);
    resumeButton = new Button(MAX_X - 186, 6, MAX_X - 6, 36, false);
    soundButton = new Button (MAX_X - 180, MAX_Y - 15, MAX_X, MAX_Y);

    //timing stuff
    timer = new QTime();
    timer->start();
    startTimer(1/(double)GameEngine::MAX_FPS*1000);

    //sounds
    soundManager = new SoundManager();
    soundManager->playSound(SoundManager::TITLE);
}

MainWindow::~MainWindow(){
    while(!stars->empty()) delete stars->back(), stars->pop_back();
    delete stars;
    delete starClr;
    delete timer;
    delete levelButton;
    delete zenButton;
    delete endlessButton;
    delete resumeButton;
    delete soundButton;

    delete soundManager;
    delete engine;
}

void MainWindow::doneGame(const unsigned int score){
    resumeButton->enabled = false;
    engine->setVisible(false);
    instance->setVisible(true);
    engine->reset();
    qDebug() << "Final score was " << score;
    //highscores(score)
}

void MainWindow::pauseGame(){
    resumeButton->enabled = true;
    engine->setVisible(false);
    instance->setVisible(true);
    soundManager->playSound(muted ? SoundManager::NONE : SoundManager::TITLE);
}

void MainWindow::mousePressEvent(QMouseEvent *event){
    if (levelButton->mouseOver(currMousePos)){
        levelButton->down = true;
    }
    else if (endlessButton->mouseOver(currMousePos)){
        endlessButton->down = true;
    }
    else if (zenButton->mouseOver(currMousePos)){
        zenButton->down = true;
    }
    else if (resumeButton->mouseOver(currMousePos)){
        resumeButton->down = true;
    }
    else if (soundButton->mouseOver(currMousePos)){
        soundButton->down = true;
    }
    else if (exitButton->mouseOver(currMousePos)){
        exitButton->down = true;
    }
}
void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    if (levelButton->mouseOver(currMousePos) && levelButton->down){
        gameType = LevelManager::LEVELED;
        launchGame();
    }
    else if(endlessButton->mouseOver(currMousePos) && endlessButton->down){
        gameType = LevelManager::NONSTOP;
        launchGame();
    }
    else if(zenButton->mouseOver(currMousePos) && zenButton->down){
        gameType = LevelManager::ZEN;
        launchGame();
    }
    else if (resumeButton->mouseOver(currMousePos) && resumeButton->down){
        instance->setVisible(false);
        engine->setVisible(true);
        engine->resume();
        soundManager = new SoundManager();
        soundManager->playSound(SoundManager::GAME);
    }
    else if (soundButton->mouseOver(currMousePos) && soundButton->down){
        muted = !muted;
        soundManager->playSound(muted ? SoundManager::NONE : SoundManager::TITLE);
    }
    else if (exitButton->mouseOver(currMousePos) && exitButton->down){
        exit(0);
    }

    //reset button states
    levelButton->down = false;
    endlessButton->down = false;
    zenButton->down = false;
    exitButton->down = false;
    resumeButton->down = false;
    soundButton->down = false;
}

void MainWindow::initializeGL(){
    //get current OpenGL context
    makeCurrent();

    //window stuff
    glViewport(0, 0, MAX_X, MAX_Y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, MAX_X, 0, MAX_Y); //orthogonal, not perspective
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glEnable(GL_BLEND);

    //load textures
    fontTex = loadTexture(":/Images/font.png");
    titleTex = loadTexture(":/Images/TITLE2.png");
    cursorTex = loadTexture(":/Images/Cursor.png");
}

void MainWindow::timerEvent(QTimerEvent *){
    update();
    updateGL();
}

void MainWindow::update(){
    double deltaTime = timer->restart()/(float)1000;
    if(!engine->isVisible()){
        soundManager->update(deltaTime);
    }
    //get mouse position
    currMousePos = mapFromGlobal(QCursor::pos());
    currMousePos.setY(MAX_Y - currMousePos.y());

    //move stars
    const double panX = STAR_PAN_X * deltaTime;
    const double panY = STAR_PAN_Y * deltaTime;
    for (int i = 0 ; i < NUM_STARS ; i++){
        stars->at(i)->x += panX/(float)stars->at(i)->dist;
        stars->at(i)->y += panY/(float)stars->at(i)->dist;
        //wrap the star around when panned offscreen
        if (stars->at(i)->x > MAX_X){
            stars->at(i)->x -= MAX_X;
        }
        else if (stars->at(i)->x < 0){
            stars->at(i)->x = MAX_X - stars->at(i)->x;
        }
        if (stars->at(i)->y > MAX_Y){
            stars->at(i)->y -= MAX_Y;
        }
        else if (stars->at(i)->y < 0){
            stars->at(i)->y = MAX_Y - stars->at(i)->y;
        }
    }
}

//draw the scene
void MainWindow::paintGL(){
    //clear the screen
    glClear(GL_COLOR_BUFFER_BIT);

    //draw the stars
    glBegin(GL_POINTS);
    for(int i = 0; i < NUM_STARS; i++){
        glVertex2d(stars->at(i)->x, stars->at(i)->y);
    }
    glEnd();

    //draw the button options
    Util::drawString("LEVEL MODE", 300, 300, fontTex, true, false, 2, 2, true);
    Util::drawString("ENDLESS PLAY", 300, 200, fontTex, true, false, 2, 2, true);
    Util::drawString("ZEN MODE", 300, 100, fontTex, true, false, 2, 2, true);
    Util::drawString("EXIT",6, 6, fontTex, false, false, 2, 2, true);
    Util::drawString("TOGGLE SOUND", MAX_X - 180, MAX_Y - 15, fontTex);
    if (resumeButton->enabled){
        Util::drawString("RESUME", MAX_X - 186, 6, fontTex, false, false, 2, 2, true);
    }

    //hovering stuff
    if (levelButton->mouseOver(currMousePos)){
        Util::drawTexture(levelButton->x1 - CURSOR_OFFSET - 23, levelButton->y1 - 5, levelButton->x1 - CURSOR_OFFSET, levelButton->y2 + 5, cursorTex);
        Util::drawTexture(levelButton->x2 + CURSOR_OFFSET + 23, levelButton->y1 - 5, levelButton->x2 + CURSOR_OFFSET, levelButton->y2 + 5, cursorTex);
    }
    if (endlessButton->mouseOver(currMousePos)){
        Util::drawTexture(endlessButton->x1 - CURSOR_OFFSET - 23, endlessButton->y1 - 5, endlessButton->x1 - CURSOR_OFFSET, endlessButton->y2 + 5, cursorTex);
        Util::drawTexture(endlessButton->x2 + CURSOR_OFFSET + 23, endlessButton->y1 - 5, endlessButton->x2 + CURSOR_OFFSET, endlessButton->y2 + 5, cursorTex);
    }
    if (zenButton->mouseOver(currMousePos)){
        Util::drawTexture(zenButton->x1 - CURSOR_OFFSET - 23, zenButton->y1 - 5, zenButton->x1 - CURSOR_OFFSET, zenButton->y2 + 5, cursorTex);
        Util::drawTexture(zenButton->x2 + CURSOR_OFFSET + 23, zenButton->y1 - 5, zenButton->x2 + CURSOR_OFFSET, zenButton->y2 + 5, cursorTex);
    }
    if (exitButton->mouseOver(currMousePos)){
        Util::drawTexture(exitButton->x2 + CURSOR_OFFSET + 23, exitButton->y1 - 5, exitButton->x2 + CURSOR_OFFSET, exitButton->y2 + 5, cursorTex);
    }
    if (resumeButton->mouseOver(currMousePos)){
        Util::drawTexture(resumeButton->x1 - CURSOR_OFFSET - 23, resumeButton->y1 - 5, resumeButton->x1 - CURSOR_OFFSET, resumeButton->y2 + 5, cursorTex);
    }


    //draw the title ew ew hard code. IMG has width 552, height 106
    Util::drawTexture((MAX_X-552)/2, (MAX_Y-50-106), (((MAX_X-552)/2)+552), (MAX_Y-50), titleTex);
}

//launch the game
void MainWindow::launchGame(){
    if (resumeButton->enabled) engine->reset();
    instance->setVisible(false);
    engine->setVisible(true);
    engine->start(gameType);
    soundManager->playSound(muted ? SoundManager::NONE : SoundManager::GAME);
}

//init the stars
void MainWindow::initStars(){
    starClr = new QColor(255, 255, 255);
    stars = new std::vector<mStar*>;
    stars->reserve(NUM_STARS);
    for(int i = 0; i < NUM_STARS; i++){
        stars->push_back(new mStar(qrand() % MAX_X, qrand() % MAX_Y, Util::randInt(1,MAX_DIST)));
    }
}

//load a texture from a file
GLuint MainWindow::loadTexture(const char* c){
    GLuint ret;
    QImage tex;
    tex = QGLWidget::convertToGLFormat(QImage(c));
    if (tex.isNull()){
        qDebug() << "Error loading texture" << c;
        return 0;
    }
    //save settings
    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_TEXTURE_2D);
    //generate texture slot
    glGenTextures(1, &ret);
    //bind, load, unbind texture
    glBindTexture(GL_TEXTURE_2D, ret);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
    glBindTexture(GL_TEXTURE_2D, 0);
    //restore settings
    glPopAttrib();
    return ret;
}

//ENGINE STUFF (refactor this?)
//engine mouse stuff
QPoint MainWindow::getMousePos(){
    return engine->getMousePos();
}
Qt::MouseButtons MainWindow::getMouseState(){
    return engine->getMouseState();
}
//engine keyboard stuff
bool MainWindow::getKeyPressed(int k){
    return engine->getKeyPressed(k);
}

void MainWindow::shakeGameScreen(double time, int amt){
    engine->shakeScreen(time, amt);
}
