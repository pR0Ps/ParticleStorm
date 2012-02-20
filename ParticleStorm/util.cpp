#include "util.h"
#include <QImage>
#include <sstream>

Util::Util(){}

//distance between 2 points
double Util::distance(double x1, double y1, double x2, double y2){
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}

//returns the amount of scale to apply based on a frame to make the object pulse
float Util::getScaleByFrame(int frame, int steps, float min, float max, bool cycle){
    if (cycle) steps/= 2; //scale up and down, therefore half the steps to scale to values
    if (!cycle || (frame % (steps * 2)) <= (frame % steps))
        frame = (frame % steps);
    else
        frame = (frame % (steps * 2)) - (frame % steps) * 2 - 1;
    return frame/(float)(steps/(float)(max - min)) + min;
}

//draws a box
void Util::drawBox(double x1, double y1, double x2, double y2, bool fill, const QColor *clr){
    //save current color
    if (clr != NULL){
        glPushAttrib(GL_CURRENT_BIT);
        glColor3d(clr->red(), clr->green(), clr->blue());
    }
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

    //restore old color
    if (clr != NULL) glPopAttrib();
}

//draw a jagged line from point to point
void Util::drawJaggedLine(double x1, double y1, double x2, double y2, double var, int seg_len, double macro_var, int macro_seg_len, const QColor *clr){
    if (clr != NULL){
        glPushAttrib(GL_CURRENT_BIT);
        glColor3d(clr->red(), clr->green(), clr->blue());
    }
    if (x1 != x2 || y1 != y2){
        double old_x = x1;
        double old_y = y1;
        const double dist = distance (x1, y1, x2, y2);
        const double vx = (x2 - x1) / dist;
        const double vy = (y2 - y1) / dist;
        double tmp_x = 0;
        double tmp_y = 0;
        int sway_x = 0;
        int sway_y = 0;
        int i = 1;
        while(true){
            i += qrand() % flr(macro_var/1.5) + macro_var;
            if (i >= (dist - macro_seg_len) or (i <= 0))
                break;
            tmp_x = vx * i + x1 + qrand() % flr(macro_var) - macro_var/(double)2 + sway_x * (i / dist - 1);
            tmp_y = vy * i + y1 + qrand() % flr(macro_var) - macro_var/(double)2 + sway_y * (i / dist - 1);
            drawJaggedLineHelper (old_x, old_y, tmp_x, tmp_y, var, seg_len);
            old_x = tmp_x;
            old_y = tmp_y;
            sway_x += qrand() % flr(macro_var) - macro_var/2;
            sway_y += qrand() % flr(macro_var) - macro_var/2;
        }
        drawJaggedLineHelper (old_x, old_y, x2, y2, var, seg_len);
    }
    if (clr != NULL) glPopAttrib();
}

//helper function (draws the micro lines of drawJaggedLine)
void Util::drawJaggedLineHelper(double x1, double y1, double x2, double y2, double var, int seg_len){
    if (x1 != x2 || y1 != y2){
        double old_x = x1;
        double old_y = y1;
        const double dist = distance (x1, y1, x2, y2);
        const double vx = (x2 - x1) / dist;
        const double vy = (y2 - y1) / dist;
        double tmp_x = 0;
        double tmp_y = 0;
        //dist + 0.5 is a substitute for rounding
        for (unsigned int i = 0 ; i < dist + 0.5 ; i += seg_len){
            tmp_x = vx * i + x1 + (qrand() % flr(var)) - var/(double)2;
            tmp_y = vy * i + y1 + (qrand() % flr(var)) - var/(double)2;
            glBegin(GL_LINE);
                glVertex2d(old_x, old_y);
                glVertex2d(tmp_x, tmp_y);
            glEnd();
            old_x = temp_x;
            old_y = temp_y;
        }
        glBegin(GL_LINE);
            glVertex2d(old_x, old_y);
            glVertex2d(x2, y2);
        glEnd();
    }
}

//draws a meter (life guage, mana gauge, etc)
void Util::drawMeter(double x1, double y1, double x2, double y2, float amt, bool vert, const QColor *clr){
    drawBox(x1, y1, x2, y2, false, clr); //outside
    //inside
    if (!vert) drawBox(x1 + 2, y1 + 2, x1 + 2 + (x2 - x1 - 4) * amt, y2 - 2, true, clr);
    else drawBox(x1 + 2, y1 + 2, x2 - 2, y1 + 2 + (y2 - y1 - 4) * amt, true, clr);
}

//draw a string to the screen
//note that newlines will screw with the output when it's centered (best just to use 2 calls)
void Util::drawString(std::string s, double x, double y, const GLuint tex, bool center_x, bool center_y, float scale_x, float scale_y, bool useAlpha){
    //push the current matrix to the stack and load a new one
    glPushMatrix();
    glLoadIdentity();

    //translate the matrix
    if (center_x)
        x -= s.size() * FONT_CHAR_DIMENSION_X * scale_x/2;
    if (center_y)
        y -= FONT_CHAR_DIMENSION_Y * scale_y/2;
    glTranslated(x, y, 0);

    //scale the matrix
    glScalef(scale_x, scale_y, 1);

    if (useAlpha){
        //save the previous settting
        glPushAttrib(GL_COLOR_BUFFER_BIT);
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
            dy += FONT_CHAR_DIMENSION_Y;
        }
        else{
            //translation to target is taken care of by glTranslate
            drawChar(s.at(i), dx, -dy, tex);
            dx += FONT_CHAR_DIMENSION_X - 1;
        }
    }
    //get back old blend settings
    if (useAlpha) glPopAttrib();
    //get back the old matrix
    glPopMatrix();
}

//draws a character to the screen (call drawString instead, this is just a helper)
void Util::drawChar(char c, double x, double y, const GLuint tex){
    const int col = c % FONT_IMG_DIMENSION_X;
    const int row = FONT_IMG_DIMENSION_Y - 1 - (c / FONT_IMG_DIMENSION_Y);
    const float dim_x = FONT_CHAR_DIMENSION_X * FONT_IMG_DIMENSION_X;
    const float dim_y = FONT_CHAR_DIMENSION_Y * FONT_IMG_DIMENSION_Y;
    const float x1_tex = (col * FONT_CHAR_DIMENSION_X - 1) / dim_x;
    const float y1_tex = (row * FONT_CHAR_DIMENSION_Y + 2) / dim_y;
    const float x2_tex = ((col + 1) * FONT_CHAR_DIMENSION_X - 1) / dim_x;
    const float y2_tex = ((row + 1) * FONT_CHAR_DIMENSION_Y + 2) / dim_y;

    //this has problems sometimes with rounding. Add/subtracting small values (above) seems to fix it
    //but we may have to move to glTexCoord2i vs glTexCoord2f (and not use the drawTexture() function)
    drawTexture(x, y, x + FONT_CHAR_DIMENSION_X, y + FONT_CHAR_DIMENSION_Y, tex, x1_tex, y1_tex, x2_tex, y2_tex);

    //draw box around the character to help with testing
    drawBox(x, y, x + FONT_CHAR_DIMENSION_X, y + FONT_CHAR_DIMENSION_Y, false);
}

//Draws a texture to the screen.
//Note that the texture coords are in percents of the total texture size, not hard values
void Util::drawTexture(double x1, double y1, double x2, double y2, const GLuint tex, float x1_tex, float y1_tex, float x2_tex, float y2_tex){
    //save settings
    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_TEXTURE_2D);
    //bind texture and set parameters
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    //draw to screen
    glBegin(GL_QUADS);
        glTexCoord2f(x1_tex, y1_tex); glVertex2i(x1, y1);
        glTexCoord2f(x2_tex, y1_tex); glVertex2i(x2, y1);
        glTexCoord2f(x2_tex, y2_tex); glVertex2i(x2, y2);
        glTexCoord2f(x1_tex, y2_tex); glVertex2i(x1, y2);
    glEnd();
    //restore settings
    glPopAttrib();
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
    //save settings
    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_TEXTURE_2D);
    //generate texture slot
    glGenTextures(1, &ret);
    //bind, load, unbind texture
    glBindTexture(GL_TEXTURE_2D, ret);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
    glBindTexture(GL_TEXTURE_2D, 0);
    //restore settings
    glPopAttrib();
    return ret;
}
