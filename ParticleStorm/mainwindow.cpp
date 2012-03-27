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
    instance = this;

    //init stars
    initStars();

    //Buttons relative to texture centers
    levelButton = new Button(150, 275, 450, 325);
    endlessButton = new Button(100, 175, 500, 225);
    zenButton = new Button(175, 75, 425, 125);

    //timing stuff
    timer = new QTime();
    timer->start();
    startTimer(1/(double)GameEngine::MAX_FPS*1000);
}

MainWindow::~MainWindow(){
    while(!stars->empty()) delete stars->back(), stars->pop_back();
    delete stars;
    delete starClr;
    delete timer;
    delete levelButton;
    delete zenButton;
    delete endlessButton;
    delete engine;
}

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
}
void MainWindow::mouseReleaseEvent(QMouseEvent *event){
    if (levelButton->mouseOver(currMousePos) && levelButton->down){
        instance->setVisible(false);
        engine->setVisible(true);
        engine->start(LevelManager::LEVELED);
    }
    else if(endlessButton->mouseOver(currMousePos) && endlessButton->down){
        instance->setVisible(false);
        engine->setVisible(true);
        engine->start(LevelManager::NONSTOP);
    }
    else if(zenButton->mouseOver(currMousePos) && zenButton->down){
        instance->setVisible(false);
        engine->setVisible(true);
        engine->start(LevelManager::ZEN);
    }

    //reset button states
    levelButton->down = false;
    endlessButton->down = false;
    zenButton->down = false;
}

void MainWindow::doneGame(const unsigned int score){
    engine->setVisible(false);
    instance->setVisible(true);
    engine->reset();
    qDebug() << "Final score was " << score;
    //highscores(score)
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
}



void MainWindow::timerEvent(QTimerEvent *){
    update();
    updateGL();
}

void MainWindow::update(){
    double deltaTime = timer->restart()/(float)1000;

    //get mouse position
    currMousePos = mapFromGlobal(QCursor::pos());
    currMousePos.setY(MAX_Y - currMousePos.y());

    //hovering stuff
    if (levelButton->mouseOver(currMousePos)){
        //draw cursor image
    }

    //move stars
    const double panX = 500 * deltaTime;
    const double panY = 0 * deltaTime;
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
    Util::drawString("LEVEL MODE", 300, 300, fontTex, true, true, 2, 2, true);
    Util::drawString("ENDLESS PLAY", 300, 200, fontTex, true, true, 2, 2, true);
    Util::drawString("ZEN MODE", 300, 100, fontTex, true, true, 2, 2, true);

    //draw the title ew ew hard code. IMG has width 552, height 106
    Util::drawTexture((MAX_X-552)/2, (MAX_Y-50-106), (((MAX_X-552)/2)+552), (MAX_Y-50), titleTex);
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
