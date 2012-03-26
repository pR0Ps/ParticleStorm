#include <QPoint>
#include "mainwindow.h"


Button::Button(double width, double height, double centerX, double centerY){
    this->centerX = centerX;
    this->centerY = centerY;
    this->height = height;
    this->width = width;
}

bool Button::mouseHover(){
    QPoint mousePossy = MainWindow::getInstance()->getMousePos();
    if((mousePossy.x() <= centerX + width/2) && (mousePossy.x() >= centerX - width/2) && (mousePossy.y() <= centerY + height/2) && (mousePossy.y() >= centerY - height/2)){
        return true;
    }
    else{
        return false;
    }
}
