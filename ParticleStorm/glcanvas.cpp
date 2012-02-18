#include "glcanvas.h"
#include "util.h"
#include <QGLWidget>
#include <QtOpenGL>
#include <time.h>

GLCanvas::GLCanvas(QWidget *parent) : QGLWidget(parent){
    setFixedSize(MAX_X, MAX_Y);
    setAutoFillBackground(false);

    //pseudo-randomness
    qsrand((uint)time(NULL));

    //init some values
    for (int i = 0 ; i < 6 ; i++){
        coords[i] = qrand() % (i % 2 == 0 ? MAX_X : MAX_Y);
        vels[i] = (qrand() % 5) + 5;
    }

    //start the timer (sets off the frameloop via timerEvent)
    startTimer(1/MAX_FPS*1000);
}

//destructor
GLCanvas::~GLCanvas(){
    delete fbo;
}

void GLCanvas::initializeGL(){
    //get current OpenGL context
    makeCurrent();

    //current version of OpenGL
    qDebug() << "Current OpenGL version: " << (char*)glGetString(GL_VERSION);

    //window stuff
    glViewport(0, 0, MAX_X, MAX_Y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, MAX_X, 0, MAX_Y); //orthogonal, not perspective
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    //init and clear the framebuffer
    QGLFramebufferObjectFormat format;
    format.setAttachment(QGLFramebufferObject::NoAttachment);
    format.setInternalTextureFormat(GL_RGB);
    format.setTextureTarget(GL_TEXTURE_2D);
    fbo = new QGLFramebufferObject(MAX_X, MAX_Y, format);
    fbo->bind();
        glColor3i(0, 0, 0);
        glClear(GL_COLOR_BUFFER_BIT);
    fbo->release();

}

//draw the contents of the framebuffer
void GLCanvas::drawFramebuffer(){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, fbo->texture());
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex2i(0, 0);
        glTexCoord2f(0, 1); glVertex2i(0, MAX_Y);
        glTexCoord2f(1, 1); glVertex2i(MAX_X, MAX_Y);
        glTexCoord2f(1, 0); glVertex2i(MAX_X, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

//blur the frame
void GLCanvas::doBlur(){
    glColor3f(0, 0, 0);
    for (int i = 0 ; i < LINES_PER_BLUR ; i++){
        glBegin(GL_LINE);
            glVertex2i(qrand() % MAX_X, qrand() % MAX_Y);
            glVertex2i(qrand() % MAX_X, qrand() % MAX_Y);
        glEnd();
    }
}

//test scene
void GLCanvas::drawScene(){
    //red box around the window
    Util::drawBox(10, 10, MAX_X - 10, MAX_Y - 10, false, QColor(255, 0, 0));

    //the triangle
    glBegin(GL_TRIANGLES);
        glColor3f(1, 0, 0); glVertex2i(coords[0], coords[1]);
        glColor3f(0, 1, 0); glVertex2i(coords[2], coords[3]);
        glColor3f(0, 0, 1); glVertex2i(coords[4], coords[5]);
    glEnd();
}

//update game logic - automatically called
void GLCanvas::update(){
    //basic bounds
    for (int i = 0 ; i < 6 ; i++){
        if (coords[i] > (i % 2 == 0 ? MAX_X : MAX_Y)){
            vels[i] *= -1;
            coords[i] = (i % 2 == 0 ? MAX_X : MAX_Y);
        }
        else if (coords[i] < 0){
            vels[i] *= -1;
            coords[i] = 0;
        }
        //apply velocity
        coords[i] += vels[i];
    }
}

//draws everything - automatically called
void GLCanvas::paintGL(){
    /*Draw method:
      To framebuffer (will be blurred):
        Old framebuffer (don't erase it)
        Blur
        Stars
        Shrapnel
        Powerups
        Player (maybe implement selective blurring)
      To screen (no blurring):
        Current framebuffer
        Particles
        Enemies
        HUD
    */

    //draw to the framebuffer
    fbo->bind();
        doBlur();
        drawScene();
    fbo->release();

    drawFramebuffer();

}

