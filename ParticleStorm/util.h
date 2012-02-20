#ifndef UTIL_H
#define UTIL_H

#include <QtOpenGL>
#include <QColor>
#include <math.h>

#define FONT_IMG_DIMENSION_X 16
#define FONT_IMG_DIMENSION_Y 16
#define FONT_CHAR_DIMENSION_X 16
#define FONT_CHAR_DIMENSION_Y 16

//utility class that provides static functions for calculations,
//drawing methods, and more.

class Util{
private:
    Util();
public:
    //calculations
    static double distance(double x1, double y1, double x2, double y2);
    static int flr (double d){return static_cast<int>(floor(d));} //because floor doesn't return an int (WRYYY?)
    static float getScaleByFrame(int frame, int steps, float min, float max, bool smooth = true);
    static double magnitude(double x, double y) {return distance(0, 0, x, y);}

    //drawing
    static void drawBox(double x1, double y1, double x2, double y2, bool fill, const QColor *clr = NULL);
    static void drawJaggedLine(double x1, double y1, double x2, double y2, double var, int seg_len, double macro_var, int macro_seg_len, const QColor *clr = NULL);
    static void drawMeter(double x1, double y1, double x2, double y2, float amt, bool vert = false, const QColor *clr = NULL);
    static void drawString(std::string s, double x, double y, const GLuint tex, bool center_x = false, bool center_y = false, float scale_x = 1, float scale_y = 1, bool useAlpha = true);
    static void drawTexture(double x1, double y1, double x2, double y2, const GLuint tex, float x1_tex = 0, float y1_tex = 0, float x2_tex = 1, float y2_tex = 1);

    //formatting
    static std::string doubleToString(double d, unsigned int width = 0, unsigned int precision = 2);

    //loading textures
    static GLuint loadTextureFromFile(const char* c);
    static GLuint loadTextureFromFile(std::string s) {return loadTextureFromFile(s.c_str());}

private:
    //helper functions
    static void drawChar (char c, double x, double y, const GLuint tex);
    static void drawJaggedLineHelper(double x1, double y1, double x2, double y2, double var, int seg_len);
};

#endif // GLOBALS_H
