#ifndef GLCANVAS_H
#define GLCANVAS_H

#include <QGLWidget>
#include <QtOpenGL>

#define MAX_X 1024
#define MAX_Y 768
#define MAX_FPS 60
#define LINES_PER_FADE 400

class GLCanvas : public QGLWidget{
public:
    explicit GLCanvas(QWidget *parent = 0);
    ~GLCanvas();

    //main frameloop (update game, render frame)
    void timerEvent(QTimerEvent *){update();updateGL();}

private:
    void drawFramebuffer();
    void doFade(); //fade the previous screen every frame
    void drawHUD();
    void update(); //update the logic for all the objects

    QGLFramebufferObject *fbo; //offscreen framebuffer

    //testing stuff
    void drawScene();
    int coords[6];
    int vels[6];

protected:
    //overrides
    void initializeGL();
    void paintGL();
};

#endif // GLCANVAS_H
