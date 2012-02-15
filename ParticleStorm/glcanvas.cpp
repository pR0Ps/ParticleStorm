#include "glcanvas.h"
#include <QGLWidget>

GLCanvas::GLCanvas(QWidget *parent) : QGLWidget(parent){
    setBaseSize(600, 600);
    setAutoFillBackground(false);
    glEnable(GL_DEPTH_TEST);
}

void GLCanvas::update(){
    //update stuff here
    updateGL(); //calls the paintGL function
}

void GLCanvas::resizeGL(int w, int h){
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0f, float(w)/float(h), 1.0f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

//draws things
void GLCanvas::paintGL(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(0.0, 1.0, 6.0, //Position
              0.0, 0.0, 0.0, //Where looking
              0.0, 1.0, 0.0); //Up vector

    glBegin(GL_TRIANGLES);
        glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
        glVertex3f(2.0f, 2.5f, -1.0f);
        glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
        glVertex3f(-3.5f, -2.5f, -1.0f);
        glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
        glVertex3f(2.0f, -4.0f, -1.0f);
    glEnd();
}
