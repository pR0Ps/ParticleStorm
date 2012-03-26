#include <QPoint>
#include "mainwindow.h"


Button::Button(double x1, double y1, double x2, double y2){
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;
}

bool Button::mouseHover(){
    QPoint mousePossy = MainWindow::getInstance()->getMousePos();
    if((mousePossy.x() <= x2) && (mousePossy.x() >= x1) && (mousePossy.y() <= y2) && (mousePossy.y() >= x2)){
        return true;
    }
    else{
        return false;
    }
}
