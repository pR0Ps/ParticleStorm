#include "util.h"
#include <math.h>
#include <QtOpenGL>

Util::Util(){}

double Util::dist(double x1, double y1, double x2, double y2){
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}
double Util::dist(double x, double y){
    return dist(0, 0, x, y);
}

void Util::drawBox(double x1, double y1, double x2, double y2, bool fill, QColor clr){
    if (clr.isValid()){ //possibly slow (http://www.vision.ee.ethz.ch/computing/sepp-irix/qt-3.0-mo/qcolor.html)
        glColor3d(clr.red(), clr.green(), clr.blue());
    }
    if (!fill){
        //weirdness with the LINE_LOOP
        x1 += 1;
        y2 -= 1;
        glBegin(GL_LINE_LOOP);
    }
    else{
        glBegin(GL_QUADS);
    }
        glVertex2d(x1, y1);
        glVertex2d(x2, y1);
        glVertex2d(x2, y2);
        glVertex2d(x1, y2);
    glEnd();
}

void Util::drawMeter(double x1, double y1, double x2, double y2, double amt, bool vert, QColor clr){
    drawBox(x1, y1, x2, y2, false, clr); //outside
    if (!vert){
        drawBox(x1 + 2, y1 + 2, x1 + 2 + (x2 - x1 - 4) * amt, y2 - 2, true, clr);
    }
    else{
        drawBox(x1 + 2, y1 + 2, x2 - 2, y1 + 2 + (y2 - y1 - 4) * amt, true, clr);
    }
}
