#ifndef UTIL_H
#define UTIL_H

#include <QtOpenGL>
#include <math.h>

//utility class that provides static functions for calculations,
//drawing methods, and more.

//should probably split the graphics-related stuff off into a
//seperate class

class Util{
private:
    Util();
public:

    //structs
    struct Color{
        Color(int red, int green, int blue){
            this->red = red;
            this->green = green;
            this->blue = blue;
        }
        int red, green, blue;
    };
    struct Point2D{
        Point2D (double x, double y){
            this->x = x;
            this->y = y;
        }
        double x, y;
    };

    //constants (redeclare in implementation)
    static const int CHAR_PER_ROW;
    static const int CHAR_PER_COL;
    static const int FONT_CHAR_DIMENSION_X;
    static const int FONT_CHAR_DIMENSION_Y;
    static const double PI;

    //calculations:
    static double distance(double x1, double y1, double x2, double y2);
    static double magnitude(double x, double y) {return distance(0, 0, x, y);}
    //degree versions of cos and sin
    static double cosd (double x){return cos(x * (PI/180));}
    static double sind (double x){return sin(x * (PI/180));}
    static double atand(double y,double x){return atan2(y,x)*(180/PI); }

    //random stuff
    static int randInt(int min, int max){return qrand() % (max - min + 1) + min;}

    //because floor doesn't actually return an int (WRYYY?)
    static int flr (double d){return static_cast<int>(floor(d));}
    static int flr (float f){return static_cast<int>(floor(f));}

    // Max and min functions which take doubles as parameters (the functions in
    // the std namespace only take ints).
    static double max(double x, double y);
    static double min(double x, double y);

    //point in rectangle
    static bool coordInRect(double px1, double py1, double rx1, double ry1, double rx2, double ry2);

    //get a point on a rectangle
    static QPoint randCoordOnRect(double x1, double y1, double x2, double y3);

    //calculate the intersection point between 2 lines (if any)
    static QPointF* lineIntersect(const double x1, const double y1, const double x2, const double y2,
                                 const double x3, const double y3, const double x4, double y4);

    //for pulsing objects (scale amount per frame)
    static float getScaleByFrame(unsigned long int frame, unsigned int steps,
                                 float min, float max, bool smooth = true);

    //drawing:
    static void drawBox(double x1, double y1, double x2, double y2, bool fill, const Color *clr = NULL);
    static void drawLine(double x1, double y1, double x2, double y2, const Color *clr = NULL);

    //gives a nice lightning effect
    static void drawJaggedLine(double x1, double y1, double x2, double y2,
                               const Color *clr = NULL, unsigned int var = 9, unsigned int seg_len = 15,
                               unsigned int macro_var = 60, unsigned int macro_seg_len = 90);

    //draw a meter for health/mana/whatever else
    static void drawMeter(double x1, double y1, double x2, double y2,
                          float amt, bool vert = false, const Color *clr = NULL);

    //draw an n-sided shape
    static void drawRoundShape(double x, double y, double diameter, int numsides, bool fill, const Color *clr = NULL);

    //draw an hexagon for player diplay
    static void drawGem(double x, double y, double diameter, bool fill, const Color *clr = NULL);

    //draw a string to the screen. When using the centering options, don't try to render multi-line strings (use 2 calls instead)
    static void drawString(const std::string &s, double x, double y, const GLuint tex, bool center_x = false, bool center_y = false,
                           float scale_x = 1, float scale_y = 1, bool useAlpha = true);
    static void drawChar (char c, double x, double y, const GLuint tex);

    //draw a texture to the screen. Note that the texture coords are in % of texture dimension, not hard values
    static void drawTexture(double x1, double y1, double x2, double y2,
                            GLuint tex, float x1_tex = 0, float y1_tex = 0,
                            float x2_tex = 1, float y2_tex = 1);

    //formatting:
    static std::string doubleToString(double d, unsigned int width = 0, unsigned int precision = 2);

private:
    //helper functions

    static void drawJaggedLineHelper(double x1, double y1, double x2, double y2,
                                     unsigned int var, unsigned int seg_len);
};

#endif // UTIL_H
