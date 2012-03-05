#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "gameengine.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    /*
     * This function returns the position of the mouse relative to the OpenGL
     * canvas, for the purpose of polling player input and updating the location
     * of the player's avatar.
     *
     * Note: can use the x and y member functions to get the individual
     * coordinates of the QPoint returned. Also, this member function is declared
     * static so that the Player class does not need a specific reference to get
     * the location of the player's mouse cursor.
     */
    static QPoint getMousePos();

private slots:
    void on_actionQuit_triggered();

    void on_actionStart_triggered();

private:
    Ui::MainWindow *ui;
    static GameEngine *canvas;
};

#endif // MAINWINDOW_H
