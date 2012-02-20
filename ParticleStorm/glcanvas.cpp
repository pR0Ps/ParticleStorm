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

    //init colours
    col_health = new QColor(255, 0, 0);
    col_mana = new QColor (0, 0, 255);
    col_white = new QColor (255, 255, 255);
    col_yellow = new QColor(255, 255, 0);
    col_red = new QColor(255, 0, 0);
    col_green = new QColor(0, 255, 0);
    col_blue = new QColor(0, 0, 255);

    //FPS
    framecnt = 0;
    fps = 0;
    timer = new QTime();
    timer->start();

    //start the timer (sets off the frameloop via timerEvent)
    startTimer(1/(double)MAX_FPS*1000);
}

//destructor
GLCanvas::~GLCanvas(){
    //framebuffer
    delete fbo;

    //colours
    delete col_health;
    delete col_mana;
    delete col_white;
    delete col_yellow;
    delete col_red;
    delete col_green;
    delete col_blue;
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
    glDisable(GL_ALPHA_TEST);
    glDisable(GL_DITHER);
    glDisable(GL_FOG);
    glDisable(GL_LIGHTING);
    glDisable(GL_LINE_SMOOTH); //could enable, but performance-heavy and not supported for all
    glDisable(GL_POINT_SMOOTH); //same for this
    glDisable(GL_LINE_STIPPLE);
    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_STENCIL_TEST);

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
    //draw lightning every 3 frames
    if (framecnt % 3 == 0)
        Util::drawJaggedLine(qrand() % MAX_X, qrand() % MAX_Y, qrand() % MAX_X, qrand() % MAX_Y, 9, 15, 60, 90, col_white);
    Util::drawJaggedLine(300, 500, 600, 500, 9, 15, 60, 90, col_white);

    //draw triangles
    glBegin(GL_TRIANGLES);
        glColor3f(1, 0, 0); glVertex2i(coords[0], coords[1]);
        glColor3f(0, 1, 0); glVertex2i(coords[2], coords[3]);
        glColor3f(0, 0, 1); glVertex2i(coords[4], coords[5]);
    glEnd();
}

//draws information for the player
void GLCanvas::drawHUD(){
    //demo bars for health + mana
    Util::drawMeter(20, MAX_Y - 35, 220, MAX_Y - 20, .75, false, col_health);
    Util::drawMeter(240, MAX_Y - 35, 440, MAX_Y - 20, 1, false, col_mana);
    //Score text (frames for now)
    Util::drawString("FRAME:", MAX_X - 260, MAX_Y - 25, tex_text, false, true);
    Util::drawString(Util::doubleToString(framecnt, 10, 0), MAX_X - 90, MAX_Y - 25, tex_text, true, true, 1, Util::getScaleByFrame(framecnt, 25, 1, 2));
    //FPS text
    Util::drawString("FPS: " + Util::doubleToString(fps, 4, 1), 0, 0, tex_text);
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
    if (framecnt % FPS_COUNT_FRAME_INTERVAL == 0){
        fps = FPS_COUNT_FRAME_INTERVAL/(double)(timer->restart()) * 1000;
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

