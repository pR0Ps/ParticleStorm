#include "glcanvas.h"
#include <QGLWidget>
#include <QtOpenGL>

GLCanvas::GLCanvas(QWidget *parent) : QGLWidget(parent){
    setBaseSize(600, 600);
    setMinimumSize(600, 600);
    setAutoFillBackground(false);
    glDisable(GL_DEPTH_TEST);
    max_screen = 600;

    qsrand((uint)8794536);

    //pseudo-randomness
    for (int i = 0 ; i < 6 ; i++){
        coords[i] = qrand() % 600;
        vels[i] = (qrand() % 5) + 5;
    }

    //start the timer (calls update every time it finishes)
    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1);
}

void GLCanvas::update(){
    //basic bounds
    for (int i = 0 ; i < 6 ; i++){
        if (coords[i] > max_screen){
            vels[i] *= -1;
            coords[i] = max_screen;
        }
        else if (coords[i] < 0){
            vels[i] *= -1;
            coords[i] = 0;
        }
        //apply velocity
        coords[i] += vels[i];
    }
    updateGL(); //calls the paintGL function
}

void GLCanvas::resizeGL(int w, int h){
    max_screen = (w > h ? h : w);

    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h); //orthogonal, not perspective
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//draws things - Don't call it, call updateGL() instead
void GLCanvas::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    //red box around the square used for the animation
    glBegin(GL_LINE_LOOP);
        glColor3f(1, 0, 0);
        glVertex2d(1, 1);
        glVertex2d(1, max_screen - 1);
        glVertex2d(max_screen - 1, max_screen - 1);
        glVertex2d(max_screen - 1, 1);
    glEnd();

    //the triangle
    glBegin(GL_TRIANGLES);
        glColor3f(1, 0, 0);
        glVertex2i(coords[0], coords[1]);
        glColor3f(0, 1, 0);
        glVertex2i(coords[2], coords[3]);
        glColor3f(0, 0, 1);
        glVertex2i(coords[4], coords[5]);
    glEnd();
}
