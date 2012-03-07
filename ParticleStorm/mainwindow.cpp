#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gameengine.h"

MainWindow* MainWindow::instance = NULL;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    setWindowTitle("Particle Storm");

    //set up the game engine
    engine = new GameEngine();
    engine->setMouseTracking(true);

    instance = this;
}

MainWindow::~MainWindow(){
    delete ui;
    delete engine;
}

QPoint MainWindow::getMousePos(){
    // The pos function returns a QPoint representing the global coordinates of
    // the mouse and the mapFromGlobal function converts this to a point relative
    // to a QWidget, in this case the OpenGL canvas.
    return engine->mapFromGlobal(QCursor::pos());
}

Qt::MouseButtons MainWindow::getMouseState(){
    return engine->getMouseState();
}

void MainWindow::doneGame(const unsigned int score){
    engine->setVisible(false);
    instance->setVisible(true);
    engine->reset();
    qDebug() << "Final score was " << score;
    //highscores(score)
}

void MainWindow::on_actionQuit_triggered(){
    exit(0);
}

void MainWindow::on_actionStart_triggered(){
    instance->setVisible(false);
    engine->setVisible(true);
    engine->start();
}
