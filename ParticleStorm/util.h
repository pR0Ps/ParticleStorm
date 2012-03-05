#ifndef UTIL_H
#define UTIL_H

#include <QtOpenGL>
#include <QColor>
#include <math.h>

//utility class that provides static functions for calculations,
//drawing methods, and more.

//should probably split the graphics-related stuff off into a
//seperate class

class Util{
private:
    Util();
public:
    //constants
    static const int FONT_IMG_DIMENSION_X = 16;
    static const int FONT_IMG_DIMENSION_Y = 16;
    static const int FONT_CHAR_DIMENSION_X = 16;
    static const int FONT_CHAR_DIMENSION_Y = 16;
    static const double PI = 3.141592654;

    //calculations:
    static double distance(double x1, double y1, double x2, double y2);
    static double magnitude(double x, double y) {return distance(0, 0, x, y);}
    //degree versions of cos and sin
    static double cosd (double x){return cos(x * (180 / PI));}
    static double sind (double x){return sin(x * (180 / PI));}

    //because floor doesn't actually return an int (WRYYY?)
    static int flr (double d){return static_cast<int>(floor(d));}
    static int flr (float f){return static_cast<int>(floor(f));}

    //for pulsing objects (scale amount per frame)
    static float getScaleByFrame(unsigned long int frame, unsigned int steps,
                                 float min, float max, bool smooth = true);

    //drawing:
    static void drawBox(double x1, double y1, double x2, double y2, bool fill, const QColor *clr = NULL);

    //gives a nice lightning effect
    static void drawJaggedLine(double x1, double y1, double x2, double y2,
                               const QColor *clr = NULL, unsigned int var = 9, unsigned int seg_len = 15,
                               unsigned int macro_var = 60, unsigned int macro_seg_len = 90);

    //draw a meter for health/mana/whatever else
    static void drawMeter(double x1, double y1, double x2, double y2,
                          float amt, bool vert = false, const QColor *clr = NULL);

    //draw a string to the screen. When using the centering options, don't try to render multi-line strings (use 2 calls instead)
    static void drawString(const std::string &s, double x, double y, const GLuint tex, bool center_x = false, bool center_y = false,
                           float scale_x = 1, float scale_y = 1, bool useAlpha = true);

    //draw a texture to the screen. Note that the texture coords are in % of texture dimension, not hard values
    static void drawTexture(double x1, double y1, double x2, double y2,
                            GLuint tex, float x1_tex = 0, float y1_tex = 0,
                            float x2_tex = 1, float y2_tex = 1);

    //formatting:
    static std::string doubleToString(double d, unsigned int width = 0, unsigned int precision = 2);

private:
    //helper functions
    static void drawChar (char c, double x, double y, const GLuint tex);

    static void drawJaggedLineHelper(double x1, double y1, double x2, double y2,
                                     unsigned int var, unsigned int seg_len);
};

#endif // UTIL_H
