#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtOpenGL>
#include <GL/glu.h>
#include <GL/gl.h>
#include <vector>
#include "gameengine.h"

class MainWindow : public QGLWidget{
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static const int MAX_X = 600;
    static const int MAX_Y = 600;
    static const int MAX_DIST = 10;
    static const int NUM_STARS = 255;

    /*
     * This function returns the position of the mouse relative to the OpenGL
     * canvas, for the purpose of polling player input and updating the location
     * of the player's avatar.
     *
     * Note: can use the x and y member functions to get the individual
     * coordinates of the QPoint returned.
     */
    QPoint getMousePos();
    Qt::MouseButtons getMouseState();
    bool keyPressed(GameEngine::Keys k);

    //get this instance
    static MainWindow* getInstance(){return instance;}

    void doneGame(unsigned int score);

private:
    GameEngine *engine;

    struct mStar{
        double x, x_old;
        double y, y_old;
        double dist;
        mStar(double x, double y, double dist){
            this->x = x;
            this->y = y;
            x_old = x;
            y_old = y;
            this->dist = dist;
        }
    };

    std::vector<mStar*> *starVect;

    void fillStar();
    const QColor *clr;
    int menuClock;
    QTime *timer;
    static MainWindow *instance;

protected:
    void initializeGL();
    void paintGL();
    void timerEvent(QTimerEvent *);
    void update();
};

#endif // MAINWINDOW_H
