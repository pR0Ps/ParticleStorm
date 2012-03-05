#include "texturemanager.h"
#include <QImage>
#include <QtOpenGL>

TextureManager* TextureManager::instance = NULL;

TextureManager::TextureManager()
{
    if(instance){
        qDebug() << "Only one instance of TextureManager allowed";
        return;
    }

    //defined textures (KEEP IN SYNC WITH ENUM)
    const char* tex_files[] = {
        ":/Images/font.png",
        ":/Images/Player.png"
    };

    //load the textures
    textures = new std::vector<GLuint>;
    for (unsigned int i = 0 ; i < sizeof(tex_files)/sizeof(char*) ; i++){
        textures->push_back(loadTextureFromFile(tex_files[i]));
    }

    //set up self reference
    instance = this;
}

TextureManager::~TextureManager()
{
    if(instance){
        //unload all textures
        while(!textures->empty()){
            glDeleteTextures(1, &textures->back());
            textures->pop_back();
        }
        delete textures;
    }
}

//loads an image from a file and returns the OpenGL handle for it
GLuint TextureManager::loadTextureFromFile(const char* c){
    GLuint ret;
    QImage tex;
    tex = QGLWidget::convertToGLFormat(QImage(c));
    if (tex.isNull()){
        qDebug() << "Error loading texture" << c;
        return 0;
    }
    //save settings
    glPushAttrib(GL_ENABLE_BIT);
    glEnable(GL_TEXTURE_2D);
    //generate texture slot
    glGenTextures(1, &ret);
    //bind, load, unbind texture
    glBindTexture(GL_TEXTURE_2D, ret);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex.width(), tex.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, tex.bits());
    glBindTexture(GL_TEXTURE_2D, 0);
    //restore settings
    glPopAttrib();
    return ret;
}

GLuint TextureManager::getTexture(Texture t){
    return textures->at(t);
}
