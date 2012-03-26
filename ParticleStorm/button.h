#ifndef BUTTON_H
#define BUTTON_H

class Button{
public:
    Button();
    //~Button();
    Button(double width, double height, double centerX, double centerY);
    bool mouseHover();
private:
    double centerX;
    double centerY;
    double width;
    double height;
};

#endif // BUTTON_H
