#ifndef GLCANVAS_H
#define GLCANVAS_H

#include <QtOpenGL>
#include <GL/glu.h>
#include <GL/gl.h>
#include <QTime>

#include "objectmanager.h"

class GLCanvas : public QGLWidget{
public:
    explicit GLCanvas(QWidget *parent = 0);
    ~GLCanvas();

    //constants
    static const int MAX_X = 1024;
    static const int MAX_Y = 768;
    static const int MAX_FPS = 50;
    static const int FPS_COUNT_FRAME_INTERVAL = 100;
    static const int LINES_PER_FADE = 400;
    static const int FADE_BORDER_AMT = 15;

private:
    //main frameloop (update game, render frame)
    void timerEvent(QTimerEvent *){update();updateGL();}

    void doFade(); //fade the previous screen every frame
    void drawHUD();
    void update(); //update the logic for all the objects

    QGLFramebufferObject *fbo; //offscreen framebuffer
    ObjectManager *objectManager; //manages all the objects in the game

    //testing stuff
    void drawScene();
    int coords[6];
    int vels[6];

    //FPS stuff
    unsigned long int framecnt;
    QTime *timer;
    float fps;

    //colours
    const QColor *col_white;
    const QColor *col_yellow;
    const QColor *col_red;
    const QColor *col_green;
    const QColor *col_blue;

    //textures
    GLuint tex_text;

protected:
    //overrides
    void initializeGL();
    void paintGL();
};

#endif // GLCANVAS_H
