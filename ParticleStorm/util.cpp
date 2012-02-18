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
    glBegin(fill ? GL_QUADS : GL_LINE_LOOP);
        glVertex2d(x1, y1);
        glVertex2d(x2, y1);
        glVertex2d(x2, y2);
        glVertex2d(x1, y2);
    glEnd();
}
