#include "glcanvas.h"
#include <QGLWidget>
#include <QtOpenGL>

GLCanvas::GLCanvas(QWidget *parent) : QGLWidget(parent){
    setBaseSize(600, 600);
    setMinimumSize(600, 600);
    setAutoFillBackground(false);
    glDisable(GL_DEPTH_TEST);
}

void GLCanvas::update(){
    //update stuff here
    updateGL(); //calls the paintGL function
}

void GLCanvas::resizeGL(int w, int h){
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//draws things - Don't call it, call updateGL() instead
void GLCanvas::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glBegin(GL_TRIANGLES);
        glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
        glVertex2i(0, 100);
        glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
        glVertex2i(600, 600);
        glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
        glVertex2i(200, 0);
    glEnd();
}
