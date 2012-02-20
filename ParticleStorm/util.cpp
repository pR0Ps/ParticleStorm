#include "util.h"
#include <math.h>
#include <QImage>
#include <sstream>

Util::Util(){}

//distance between 2 points
double Util::dist(double x1, double y1, double x2, double y2){
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

void Util::drawBox(double x1, double y1, double x2, double y2, bool fill, QColor clr){
    //possibly slow (http://www.vision.ee.ethz.ch/computing/sepp-irix/qt-3.0-mo/qcolor.html)
    if (clr.isValid()) glColor3d(clr.red(), clr.green(), clr.blue());
    if (!fill){
        //weirdness with the LINE_LOOP
        x1 += 1;
        y2 -= 1;
        glBegin(GL_LINE_LOOP);
    }
    else glBegin(GL_QUADS);
    //start drawing
        glVertex2d(x1, y1);
        glVertex2d(x2, y1);
        glVertex2d(x2, y2);
        glVertex2d(x1, y2);
    glEnd();
}

//draws a meter (life guage, mana gauge, etc)
void Util::drawMeter(double x1, double y1, double x2, double y2, float amt, bool vert, QColor clr){
    drawBox(x1, y1, x2, y2, false, clr); //outside
    //inside
    if (!vert) drawBox(x1 + 2, y1 + 2, x1 + 2 + (x2 - x1 - 4) * amt, y2 - 2, true, clr);
    else drawBox(x1 + 2, y1 + 2, x2 - 2, y1 + 2 + (y2 - y1 - 4) * amt, true, clr);
}

//draw a string to the screen
void Util::drawString(std::string s, double x, double y, GLuint tex, int spacing, bool useAlpha){
    if (useAlpha){
        //tell OpenGL to use the transparency of the font texture
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
    }
    //draw individual characters
    int dx = 0;
    int dy = 0;
    for(unsigned int i = 0; i < s.size(); i++){
        if(s.at(i) == '\n'){
            dx = 0;
            dy += FONT_CHAR_DIMENSIONS;
        }
        else{
            drawChar(s.at(i), x + dx, y - dy, tex);
            dx += spacing;
        }
    }
    if (useAlpha) glDisable(GL_BLEND);
}

//draws a character to the screen (call drawString instead, this is just a helper)
void Util::drawChar(char c, double x, double y, GLuint tex){
    const int row = FONT_IMG_DIMENSIONS - 1 - (c / FONT_IMG_DIMENSIONS);
    const int col = c % FONT_IMG_DIMENSIONS;
    const float dim = FONT_CHAR_DIMENSIONS * FONT_IMG_DIMENSIONS;
    const float x1_tex = (col * FONT_CHAR_DIMENSIONS) / dim;
    const float y1_tex = (row * FONT_CHAR_DIMENSIONS) / dim;
    const float x2_tex = ((col + 1) * FONT_CHAR_DIMENSIONS) / dim;
    const float y2_tex = ((row + 1) * FONT_CHAR_DIMENSIONS) / dim;

    drawTexture(x, y, x + FONT_CHAR_DIMENSIONS, y + FONT_CHAR_DIMENSIONS, tex, x1_tex, y1_tex, x2_tex, y2_tex);
}

//draws a texture to the screen. Note that the texture coords are in percents of the total texture, not hard values
void Util::drawTexture(double x1, double y1, double x2, double y2, GLuint tex, float x1_tex, float y1_tex, float x2_tex, float y2_tex){
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glBegin(GL_QUADS);
        glTexCoord2f(x1_tex, y1_tex); glVertex2i(x1, y1);
        glTexCoord2f(x2_tex, y1_tex); glVertex2i(x2, y1);
        glTexCoord2f(x2_tex, y2_tex); glVertex2i(x2, y2);
        glTexCoord2f(x1_tex, y2_tex); glVertex2i(x1, y2);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

//converts a double to a string (because it's apparently too hard for C++)
std::string Util::doubleToString(double d, unsigned int width, unsigned int precision){
    std::stringstream ss;
    ss.precision(precision);
    ss.setf(std::ios::fixed,std::ios::floatfield);
    if (width != 0) ss.width(width);
    ss << d;
    return ss.str();
}

//loads an image from a file and returns the OpenGL handle for it
GLuint Util::loadTextureFromFile(const char* c){
    GLuint ret;
    QImage tex;
    tex = QGLWidget::convertToGLFormat(QImage(c));
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &ret);
    glBindTexture(GL_TEXTURE_2D, ret);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
    return ret;
}
