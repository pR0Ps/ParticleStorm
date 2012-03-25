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
    static const int MAX_FPS = 120;
    static const int FPS_COUNT_FRAME_INTERVAL = 50;
    static const int LINES_PER_FADE = 400;
    static const int CLEAR_BORDER_AMT = 15;
    static const int GAME_OVER_FRAMES = 200;
    static const float PAN_SPEED = 0.5;

    enum Keys{
        DROP,
        PUSH,
        PULL,
        ABILITY,
        CHGABILITY //MUST BE LAST
    };

    //access mouse state
    Qt::MouseButtons getMouseState(){return mouseState;}
    QPoint getMousePos(){return mapFromGlobal(QCursor::pos());}

    //access key state
    bool keyPressed(Keys k){return currKeys[k];}
    bool keyClicked(Keys k){return currKeys[k] && !oldKeys[k];}

    //control the game engine
    void reset();
    void start();

private:
    //mouse stuff
    void mousePressEvent(QMouseEvent* event){mouseState = event->buttons();}
    void mouseReleaseEvent(QMouseEvent* event){mouseState = event->buttons();}
    Qt::MouseButtons mouseState;

    //keyboard stuff
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent * event);
    QMap<int, int> keyMap;
    bool currKeys[CHGABILITY+1];
    bool oldKeys[CHGABILITY+1];

    //call the main frameloop
    void timerEvent(QTimerEvent *);

    void doFade(); //fade the previous screen every frame
    void drawHUD();
    void update(); //update the logic for all the objects

    QGLFramebufferObject *fbo; //offscreen framebuffer
    ObjectManager *objectManager; //manages all the objects in the game
    ResourceManager *resourceManager;

    //timing stuff (FPS/deltatime)
    unsigned long int framecnt;
    double secSinceFrameInterval;
    QTime *timer;
    float fps;

    //game clock
    int gameClock;

    //game modifiers
    bool paused;
    unsigned int gameOverFrames;

    //flag for initially clearing the framebuffer
    bool initialClear;

    //stores the current amount to pan
    double panX, panY;

protected:
    //overrides
    void initializeGL();
    void paintGL();
};

#endif // GAMEENGINE_H
