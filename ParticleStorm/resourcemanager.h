#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <QtOpenGL/QGLWidget>
#include <QImage>
#include <QSound>

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
    enum Sound{
        music



  };

    //get instance
    static ResourceManager* getInstance(){return instance;}

    //get a texture
    GLuint getTexture(Texture t);

    //get colours
    const QColor* getColourScale(float f);
    const QColor* getColour (Colours c);
    void playMainMusic(double deltaTime);

private:
    //the files to load textures from
    static const char* tex_files[];
    static const char* music_sounds[];

    //the array of textures
    std::vector<GLuint> *textures;

    //holds array of colours
    std::vector<const QColor*> *colours;

   //holds the sounds
    std::vector<QSound*> *sound;

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
