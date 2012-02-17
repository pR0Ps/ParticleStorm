#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "glcanvas.h"

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

void MainWindow::on_actionQuit_triggered()
{
    exit(0);
}

void MainWindow::on_actionStart_triggered()
{
    this->hide();
    canvas = new GLCanvas();
    canvas -> setVisible(true);
}
