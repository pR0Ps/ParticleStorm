#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "gameengine.h"

// This must be re-declared here, outside of the header file, since it's a static
// data member.
GameEngine *MainWindow::canvas;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete canvas;
}

QPoint MainWindow::getMousePos() {
    // The pos function returns a QPoint representing the global coordinates of
    // the mouse and the mapFromGlobal function converts this to a point relative
    // to a QWidget, in this case the OpenGL canvas.
    return canvas->mapFromGlobal(QCursor::pos());
}

void MainWindow::on_actionQuit_triggered()
{
    exit(0);
}

void MainWindow::on_actionStart_triggered()
{
    this->hide();
    canvas = new GameEngine();
    canvas -> setVisible(true);
}
