#ifndef GLCANVAS_H
#define GLCANVAS_H

#include <QGLWidget>
#include <QtOpenGL>

#define MAX_X 1024
#define MAX_Y 768
#define MAX_FPS 60
#define LINES_PER_BLUR 400

class GLCanvas : public QGLWidget
{
    Q_OBJECT
public:
    explicit GLCanvas(QWidget *parent = 0);
    ~GLCanvas();

    //main frameloop (update game, render frame)
    void timerEvent(QTimerEvent *){update();updateGL();}

private:
    void drawFramebuffer();
    void doBlur();
    void drawScene();
    void update();

    //testing stuff
    int coords[6];
    int vels[6];

    QGLFramebufferObject *fbo; //use for blurring effect
    //int frameTimer; //

protected:
    void initializeGL();
    void paintGL();

signals:

};

#endif // GLCANVAS_H
