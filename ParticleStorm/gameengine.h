#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <QtOpenGL>
#include <GL/glu.h>
#include <GL/gl.h>
#include <QTime>

#include "objectmanager.h"
#include "resourcemanager.h"

class GameEngine : public QGLWidget{
public:
    explicit GameEngine(QWidget *parent = 0);
    ~GameEngine();

    //constants (redeclare in implementation)
    static const int MAX_X = 1024;
    static const int MAX_Y = 768;
    static const int MAX_FPS = 50;
    static const int FPS_COUNT_FRAME_INTERVAL = 100;
    static const int LINES_PER_FADE = 400;
    static const int FADE_BORDER_AMT = 15;
    static const int GAME_OVER_FRAMES = 200;

    //control the game engine
    void reset();
    void start();

private:
    //call the main frameloop
    void timerEvent(QTimerEvent *);

    void doFade(); //fade the previous screen every frame
    void drawHUD();
    void update(); //update the logic for all the objects

    QGLFramebufferObject *fbo; //offscreen framebuffer
    ObjectManager *objectManager; //manages all the objects in the game
    ResourceManager *resourceManager;

    //testing stuff
    void drawScene();
    int coords[6];
    int vels[6];

    //FPS stuff
    unsigned long int framecnt;
    QTime *timer;
    float fps;

    //game clock
    int gameClock;

    //game modifiers
    bool paused;
    unsigned int gameOverFrames;

    //colours
    const QColor *col_white;
    const QColor *col_yellow;
    const QColor *col_red;
    const QColor *col_green;
    const QColor *col_blue;
    const QColor *col_black;

protected:
    //overrides
    void initializeGL();
    void paintGL();
};

#endif // GAMEENGINE_H
