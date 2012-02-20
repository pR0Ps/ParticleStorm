#ifndef UTIL_H
#define UTIL_H

#include <QtOpenGL>
#include <QColor>

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
    static double dist(double x1, double y1, double x2, double y2);
    static double magnitude(double x, double y) {return dist(0, 0, x, y);}
    static float getScaleByFrame(int frame, int steps, float min, float max, bool smooth = true);

    //drawing
    static void drawBox(double x1, double y1, double x2, double y2, bool fill, QColor clr = QColor());
    static void drawMeter(double x1, double y1, double x2, double y2, float amt, bool vert = false, QColor clr = QColor());
    static void drawString(std::string s, double x, double y, GLuint tex, bool center_x = false, bool center_y = false, float scale_x = 1, float scale_y = 1, bool useAlpha = true);
    static void drawTexture(double x1, double y1, double x2, double y2, GLuint tex, float x1_tex = 0, float y1_tex = 0, float x2_tex = 1, float y2_tex = 1);

    //formatting
    static std::string doubleToString(double d, unsigned int width = 0, unsigned int precision = 2);

    //loading textures
    static GLuint loadTextureFromFile(const char* c);
    static GLuint loadTextureFromFile(std::string s) {return loadTextureFromFile(s.c_str());}
private:
    static void drawChar (char c, double x, double y, GLuint tex);
};

#endif // GLOBALS_H
