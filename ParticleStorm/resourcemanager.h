#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QtOpenGL/QGLWidget>
#include <QImage>
#include "util.h"

class ResourceManager
{
public:
    ResourceManager();
    ~ResourceManager();

    //number of colours to generate
    static const int NUM_GRADIENT_COLOURS;

    //NOTE: modifying the enums requires changing code in the constructor
    //textures to load
    enum Texture{
        TEXT
    };
    //colours
    enum Colours{
        WHITE, //must be first
        YELLOW,
        RED,
        GREEN,
        BLUE,
        ORANGE,
        LIGHTBLUE,
        LIGHTRED,
        PURPLE,
        BLACK //must be last
    };

    //get instance
    static ResourceManager* getInstance(){return instance;}

    //get a texture
    GLuint getTexture(Texture t);

    //get colours
    const Util::Color* getColourScale(float f);
    const Util::Color* getColour (Colours c);

private:
    //the files to load textures from
    static const char* tex_files[];

    //the array of textures
    std::vector<GLuint> *textures;

    //holds the colours
    std::vector<const Util::Color*> *gradientColours;
    std::vector<const Util::Color*> *basicColours;

    //self-reference
    static ResourceManager* instance;

    //loads a texture:
    GLuint loadTextureFromFile(const char* c);
};

#endif // RESOURCEMANAGER_H
