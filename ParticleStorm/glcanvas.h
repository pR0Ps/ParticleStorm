#ifndef GLCANVAS_H
#define GLCANVAS_H

#include <QtOpenGL>
#include <GL/glu.h>
#include <QTime>

#define MAX_X 1024
#define MAX_Y 768
#define MAX_FPS 50
#define FPS_COUNT_FRAME_INTERVAL 100
#define LINES_PER_FADE 400
#define FADE_BORDER_AMT 15

class GLCanvas : public QGLWidget{
public:
    explicit GLCanvas(QWidget *parent = 0);
    ~GLCanvas();

private:
    //main frameloop (update game, render frame)
    void timerEvent(QTimerEvent *){update();updateGL();}

    void doFade(); //fade the previous screen every frame
    void drawHUD();
    void update(); //update the logic for all the objects

    QGLFramebufferObject *fbo; //offscreen framebuffer

    //testing stuff
    void drawScene();
    int coords[6];
    int vels[6];

    //FPS stuff
    unsigned long int framecnt;
    QTime *timer;
    double fps;

    //colours
    QColor *col_health;
    QColor *col_mana;
    QColor *col_white;
    QColor *col_yellow;
    QColor *col_red;
    QColor *col_green;
    QColor *col_blue;

    //textures
    GLuint tex_text;

protected:
    //overrides
    void initializeGL();
    void paintGL();
};

#endif // GLCANVAS_H
