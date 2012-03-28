#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtOpenGL>
#include <GL/glu.h>
#include <GL/gl.h>
#include <vector>
#include <QImage>
#include "gameengine.h"
#include "soundmanager.h"

class GameEngine;

class MainWindow : public QGLWidget{
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    static const int MAX_X = 600;
    static const int MAX_Y = 600;
    static const int MAX_DIST = 10;
    static const int NUM_STARS = 255;
    static const int STAR_PAN_X = 500;
    static const int STAR_PAN_Y = 0;
    static const int CURSOR_OFFSET = 10;

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
    bool getKeyPressed(int k);

    //get this instance
    static MainWindow* getInstance(){return instance;}
    SoundManager* getSoundManager(){return soundManager;}

    int getGameType(){return gameType;}
    void shakeGameScreen(double time, int amt);
    void doneGame(unsigned int score);
    void pauseGame();

private:
    //self reference
    static MainWindow *instance;

    GameEngine *engine;
    SoundManager *soundManager;

    //star stuff
    struct mStar{
        double x;
        double y;
        double dist;
        mStar(double x, double y, double dist){
            this->x = x;
            this->y = y;
            this->dist = dist;
        }
    };
    std::vector<mStar*> *stars;
    const QColor *starClr;

    //timing stuff
    QTime *timer;

    //button stuff
    struct Button{
        Button(double x1, double y1, double x2, double y2, bool enabled = true){
            this->x1 = x1;
            this->y1 = y1;
            this->x2 = x2;
            this->y2 = y2;
            this->down = false;
            this->enabled = enabled;
        }
        bool mouseOver(QPoint point){
            if (!enabled) return false;
            return Util::coordInRect(point.x(), point.y(), x1, y1, x2, y2);
        }
        double x1;
        double y1;
        double x2;
        double y2;
        bool down;
        bool enabled;
    };
    Button *levelButton;
    Button *endlessButton;
    Button *zenButton;
    Button *exitButton;
    Button *resumeButton;
    Button *soundButton;

    QPoint currMousePos;

    //textures
    GLuint fontTex;
    GLuint titleTex;
    GLuint cursorTex;

    //zen/nonstop/leveled
    int gameType;

    //sound
    bool muted;

    void initStars();
    void launchGame();

protected:
    void initializeGL();
    void paintGL();
    void timerEvent(QTimerEvent*);
    void update();

    void mousePressEvent(QMouseEvent* event);
    void mouseReleaseEvent(QMouseEvent* event);

    GLuint loadTexture(const char* c);
};


#endif // MAINWINDOW_H
