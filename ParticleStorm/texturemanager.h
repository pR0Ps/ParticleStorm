#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <QtOpenGL/QGLWidget>
#include <QImage>

class TextureManager
{
public:
    TextureManager();
    ~TextureManager();

    //textures to load (KEEP IN SYNC WITH FILES)
    enum Texture{
        TEXT,
        PLAYER
    };

    //get instance
    static TextureManager* getInstance(){return instance;}

    //get a texture
    GLuint getTexture(Texture t);

private:
    //the files to load textures from
    static const char* tex_files[];

    //the array of textures
    std::vector<GLuint> *textures;

    //self-reference
    static TextureManager* instance;

    //loads a texture:
    GLuint loadTextureFromFile(const char* c);
};

#endif // TEXTUREMANAGER_H
