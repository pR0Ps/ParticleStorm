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

    //colours
    enum Colours{
        WHITE,
        YELLOW,
        RED,
        GREEN,
        BLUE,
        ORANGE,
        BLACK
    };

    //get instance
    static ResourceManager* getInstance(){return instance;}

    //get a texture
    GLuint getTexture(Texture t);

    //get colours
    const QColor* getColourScale(float f);
    const QColor* getColour (Colours c);

private:
    //the files to load textures from
    static const char* tex_files[];

    //the array of textures
    std::vector<GLuint> *textures;

    //holds array of colours
    std::vector<const QColor*> *colours;

    //static colours
    const QColor *white;
    const QColor *yellow;
    const QColor *red;
    const QColor *green;
    const QColor *blue;
    const QColor *orange;
    const QColor *black;

    //self-reference
    static ResourceManager* instance;

    //loads a texture:
    GLuint loadTextureFromFile(const char* c);
};

#endif // RESOURCEMANAGER_H
