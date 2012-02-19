#ifndef UTIL_H
#define UTIL_H

#include <QColor>

//utility class that provides static functions for calculations,
//drawing methods, and more.

class Util{
private:
    Util();
public:
    //calculations
    static double dist(double x1, double y1, double x2, double y2);
    static double dist(double x, double y);

    //drawing
    static void drawBox(double x1, double y1, double x2, double y2, bool fill, QColor clr = QColor());
    static void drawMeter(double x1, double y1, double x2, double y2, double amt, bool vert = false, QColor clr = QColor());
};

#endif // GLOBALS_H
