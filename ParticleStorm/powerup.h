#ifndef POWERUP_H
#define POWERUP_H

#include "gameobject.h"

using namespace std;
class Powerup : public GameObject
{
private:
    int x;
    int y;
    string type;
public:
    Powerup();

    void draw() const;
    void update();
    void applyForce(double x, double y, double mag);
};

#endif // POWERUP_H
