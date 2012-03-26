#ifndef BUTTON_H
#define BUTTON_H

class Button{
public:
    Button();
    //~Button();
    Button(double x1, double y1, double x2, double y2);
    bool mouseHover();
    double x1;
    double y1;
    double x2;
    double y2;
};

#endif // BUTTON_H
