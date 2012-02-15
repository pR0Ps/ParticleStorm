#ifndef OBJECTMANAGER_H
#define OBJECTMANAGER_H

#include <vector>

class GameObject;

class ObjectManager{
public:
    explicit ObjectManager();
    GameObject* get(int n);
    std::vector <GameObject*> getAll();
    int numberAlive();
    void draw();
    void pan(double x, double y);
    void update();
    void destroyUnused();
    void applyforce(double x, double y, double mag);
    void doCollision(const ObjectManager& om);
    void doCollision(const GameObject& ob);
};

#endif // OBJECTMANAGER_H
