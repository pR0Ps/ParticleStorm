#include "mainwindow.h"
#include "gameengine.h"
#include "util.h"
#include <QSound>
#include <QDir>
#include <iostream>

MainWindow* MainWindow::instance = NULL;

MainWindow::MainWindow(QWidget *parent) : QGLWidget(parent){

    setWindowTitle("Particle Storm");
    setFixedSize(MAX_X,MAX_Y);
    //set up the game engine
    engine = new GameEngine();
    engine->setMouseTracking(true);
    //try and do music

    //QSound music("C://Users//Daniel//CISC 320//ParticleStorm//Resources//base-loop.wav");
    //music.play();
    //QSound music("./Images/base-loop.wav");

    QString str = QDir::homePath();
    printf("%s\n", str.toAscii().constData());
    //music.play();




    clr = new QColor(255, 255, 255);
    fillStar();


    instance = this;
    menuClock = startTimer(1/(double)GameEngine::MAX_FPS*1000);
    timer = new QTime();




    //Temporary starting event
    instance->setVisible(false);
    engine->setVisible(true);
    engine->start();

}

MainWindow::~MainWindow(){
    while(!starVect->empty()) delete starVect->back(), starVect->pop_back();
    delete starVect;
    delete timer;
    delete engine;
}

//mouse stuff
QPoint MainWindow::getMousePos(){
    return engine->getMousePos();
}
Qt::MouseButtons MainWindow::getMouseState(){
    return engine->getMouseState();
}

//keyboard stuff
bool MainWindow::keyPressed(GameEngine::Keys k){
    return engine->keyPressed(k);
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

    //current version of OpenGL
    qDebug() << "Current OpenGL version:" << (char*)glGetString(GL_VERSION);

    //window stuff
    glViewport(0, 0, MAX_X, MAX_Y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, MAX_X, 0, MAX_Y); //orthogonal, not perspective
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void MainWindow::paintGL(){
    for(int i = 0; i < NUM_STARS; i++){
        Util::drawLine(starVect->at(i)->x, starVect->at(i)->y, starVect->at(i)->x_old, starVect->at(i)->y_old, clr);
    }
    glClear(GL_COLOR_BUFFER_BIT);
}

void MainWindow::timerEvent(QTimerEvent *){
    update();
    updateGL();
}


void MainWindow::update(){
    double deltaTime = timer->restart()/(float)1000;

    /*
    //move star (NOT WORKING :()
    double panx = 0.02 * deltaTime;
    for(int i = 0; i < NUM_STARS; ++i){

        starVect->at(i)->x_old = starVect->at(i)->x;
        starVect->at(i)->x += panx/starVect->at(i)->dist;

        if (starVect->at(i)->x > GameEngine::MAX_X){
            starVect->at(i)->x = 0;
            starVect->at(i)->x_old = 0;
        }
        else if (starVect->at(i)->x < 0){
            starVect->at(i)->x = GameEngine::MAX_X;
            starVect->at(i)->x_old = GameEngine::MAX_X;
        }
        if (starVect->at(i)->y > GameEngine::MAX_Y){
            starVect->at(i)->y = 0;
            starVect->at(i)->y_old = 0;
        }
        else if (starVect->at(i)->y < 0){
            starVect->at(i)->y = GameEngine::MAX_Y;
            starVect->at(i)->y_old = GameEngine::MAX_Y;
        }
    }
    */

}

void MainWindow::fillStar(){
    starVect = new vector<mStar*>;
    starVect->reserve(NUM_STARS);
    for(int i = 0; i < NUM_STARS; i++)
    {
        starVect->push_back(new mStar(qrand()%MAX_X, qrand()%MAX_Y, Util::randInt(1,10)));
    }

}
