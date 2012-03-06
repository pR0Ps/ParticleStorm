#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QtOpenGL/QGLWidget>
#include <QImage>

class ResourceManager
{
public:
    ResourceManager();
    ~ResourceManager();

    //number of colours to generate
    static const int NUM_COLOURS = 10;

    //textures to load (KEEP IN SYNC WITH FILES)
    enum Texture{
        TEXT,
        PLAYER
    };

    //get instance
    static ResourceManager* getInstance(){return instance;}

    //get a texture
    GLuint getTexture(Texture t);

    //get colours
    const QColor* getColour(float f);

private:
    //the files to load textures from
    static const char* tex_files[];

    //the array of textures
    std::vector<GLuint> *textures;

    //holds array of colours
    std::vector<const QColor*> *colours;

    //self-reference
    static ResourceManager* instance;

    //loads a texture:
    GLuint loadTextureFromFile(const char* c);
};

#endif // RESOURCEMANAGER_H
