#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <QtOpenGL>
#include <GL/glu.h>
#include <GL/gl.h>
#include <QTime>

#include "objectmanager.h"
#include "resourcemanager.h"
#include "levelmanager.h"

class LevelManager;

class GameEngine : public QGLWidget{
public:
    explicit GameEngine(QWidget *parent = 0);
    ~GameEngine();

    //declare in implementation
    static const int MAX_X;
    static const int MAX_Y;
    static const int MAX_FPS;
    static const int FPS_COUNT_FRAME_INTERVAL;
    static const int FADE_LINES_PER_SECOND;
    static const int CLEAR_BORDER_AMT;
    static const double GAME_OVER_SECONDS;
    static const float PAN_SPEED;
    static const double RESUME_GAME_LAG;

    enum Keys{
        DROP,
        PUSH,
        PULL,
        ABILITY,
        CHGABILITY,
        EXIT //MUST BE LAST
    };

    //access mouse state
    Qt::MouseButtons getMouseState(){return mouseState;}
    QPoint getMousePos() const;

    //access key state
    bool getKeyPressed(int k){return currKeys[k];}

    //control the game engine
    void reset();
    void start(int type);
    void resume();

    void shakeScreen(double time, int amt);

private:
    //mouse stuff
    void mousePressEvent(QMouseEvent* event){mouseState = event->buttons();}
    void mouseReleaseEvent(QMouseEvent* event){mouseState = event->buttons();}
    Qt::MouseButtons mouseState;

    //keyboard stuff
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent * event);
    QMap<int, int> keyMap;
    bool currKeys[EXIT+1];

    //call the main frameloop
    void timerEvent(QTimerEvent *);

    void doFade(); //fade the previous screen every frame
    void drawHUD();
    void update(double deltaTime); //update the logic for all the objects

    QGLFramebufferObject *fbo; //offscreen framebuffer
    ObjectManager *objectManager; //manages all the objects in the game
    ResourceManager *resourceManager;
    LevelManager *levelManager;

    //timing stuff (FPS/deltatime)
    unsigned long int framecnt;
    double secSinceFrameInterval;
    QTime *timer;
    float fps;

    //how much to fade the screen on next draw
    int fadeCount;

    //game clock
    int gameClock;

    //allow player to get thir bearings before putting them back into the game
    double resumeTimer;

    //screen shaking
    double shakeX;
    double shakeY;
    double shakeFactor;
    double shakeDissipation;

    //game modifiers
    bool paused;
    double gameOverTimer;
    int gameMode;

    //game over text
    std::string gameOverText;

    //flag for initially clearing the framebuffer
    bool initialClear;

    //stores the current amount to pan
    double panX, panY;

    //random spawn coordinates for enemies
    double x_start, y_start;

protected:
    //overrides
    void initializeGL();
    void paintGL();
};

#endif // GAMEENGINE_H
