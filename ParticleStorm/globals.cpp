#include "globals.h"
#include <math.h>

Globals::Globals(){}

double Globals::dist(double x1, double y1, double x2, double y2){
    return sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
}
double Globals::dist(double x, double y){
    return dist(0, 0, x, y);
}
