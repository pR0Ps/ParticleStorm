#include "glcanvas.h"
#include "util.h"
#include <time.h>

GLCanvas::GLCanvas(QWidget *parent) : QGLWidget(parent){
    setFixedSize(MAX_X, MAX_Y);
    setAutoFillBackground(false);

    //pseudo-randomness
    qsrand((uint)time(NULL));

    //init the testing values
    for (int i = 0 ; i < 6 ; i++){
        coords[i] = qrand() % (i % 2 == 0 ? MAX_X : MAX_Y);
        vels[i] = (qrand() % 5) + 5;
    }

    //FPS
    framecnt = 0;
    fps = 0;
    timer.start();

    //start the timer (sets off the frameloop via timerEvent)
    startTimer(1/(double)MAX_FPS*1000);
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

    //unneeded OpenGL features (will renable on a per-use basis)
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    //init and clear the framebuffer
    QGLFramebufferObjectFormat format;
    format.setAttachment(QGLFramebufferObject::NoAttachment);
    format.setInternalTextureFormat(GL_RGB);
    format.setTextureTarget(GL_TEXTURE_2D);
    fbo = new QGLFramebufferObject(MAX_X, MAX_Y, format);
    fbo->bind();
        glClear(GL_COLOR_BUFFER_BIT);
    fbo->release();

    //load textures
    tex_text = Util::loadTextureFromFile(":/Images/font.png");
}

//fade the frame
void GLCanvas::doFade(){
    glColor3f(0, 0, 0);

    //outsides don't get cleared very well
    Util::drawBox(0, 0, MAX_X, FADE_BORDER_AMT, true);
    Util::drawBox(0, MAX_Y - FADE_BORDER_AMT, MAX_X - FADE_BORDER_AMT, MAX_Y, true);
    Util::drawBox(0, FADE_BORDER_AMT, FADE_BORDER_AMT, MAX_Y - FADE_BORDER_AMT, true);
    Util::drawBox(MAX_X - FADE_BORDER_AMT, FADE_BORDER_AMT, MAX_X, MAX_Y, true);

    //fade the screen
    for (int i = 0 ; i < LINES_PER_FADE ; i++){
        glBegin(GL_LINE);
            glVertex2i(qrand() % MAX_X, qrand() % MAX_Y);
            glVertex2i(qrand() % MAX_X, qrand() % MAX_Y);
        glEnd();
    }
}

//test scene
void GLCanvas::drawScene(){
    glBegin(GL_TRIANGLES);
        glColor3f(1, 0, 0); glVertex2i(coords[0], coords[1]);
        glColor3f(0, 1, 0); glVertex2i(coords[2], coords[3]);
        glColor3f(0, 0, 1); glVertex2i(coords[4], coords[5]);
    glEnd();

    glBegin(GL_TRIANGLES);
        glColor3f(1, 0, 0); glVertex2i(MAX_X - coords[0], MAX_Y - coords[1]);
        glColor3f(0, 1, 0); glVertex2i(MAX_X - coords[2], MAX_Y - coords[3]);
        glColor3f(0, 0, 1); glVertex2i(MAX_X - coords[4], MAX_Y - coords[5]);
    glEnd();
}

//draws information for the player
void GLCanvas::drawHUD(){
    //demo bars for health + mana
    Util::drawMeter(20, MAX_Y - 35, 220, MAX_Y - 20, .75, false, QColor(255, 0, 0));
    Util::drawMeter(240, MAX_Y - 35, 440, MAX_Y - 20, 1, false, QColor(0, 0, 255));
    //FPS text
    Util::drawString("FPS:" + Util::doubleToString(fps, 4, 1), 0, 0, tex_text);
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
    //FPS monitoring
    framecnt++;
    if (framecnt > FPS_COUNT_FRAME_INTERVAL){
        fps = framecnt/(double)(timer.restart()) * 1000;
        framecnt = 0;
    }

    /*Draw method:
      To framebuffer:
        Old framebuffer (don't erase it)
        Fade
        Stars
        Shrapnel
        Powerups
        Player (maybe implement selective fading)
      To screen:
        Current framebuffer
        Particles
        Enemies
        HUD
    */

    //draw to the framebuffer
    fbo->bind();
        doFade();
        drawScene();
    fbo->release();

    //draw to screen
    Util::drawTexture(0, 0, MAX_X, MAX_Y, fbo->texture());
    drawHUD();
}

