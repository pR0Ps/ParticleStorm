#include "resourcemanager.h"
#include <QImage>
#include <QtOpenGL>
#include <QSound>

ResourceManager* ResourceManager::instance = NULL;

const int ResourceManager::NUM_COLOURS;
const int ResourceManager::MAIN_LOOP_TIME;
const int ResourceManager::GAME_LOOP_TIME;

ResourceManager::ResourceManager()
{
    if(instance){
        qDebug() << "Only one instance of TextureManager allowed";
        return;
    }

    //load the textures
    textures = new std::vector<GLuint>;
    textures->push_back(loadTextureFromFile(":/Images/font.png"));

    //init colour array
    gradientColours = new std::vector<const QColor*>;
    float per;
    for (int i = 0 ; i < NUM_COLOURS ; i++){
        //(gradiant from blue -> green -> red, like UI mockup)
        per = i/(float)NUM_COLOURS;
        //hard one-liner is hard
        //still a little buggy
        gradientColours->push_back(new QColor(
            (per <= .1) ? ((255/2.0)-(per/.1)*(255/2.0)) : ((per < .5) ? (0) : ((per <= .7) ? (((per-.5)/.2)*255) : (255))),
            (per <= .1) ? (0) : ((per <= .3) ? (((per-.1)/(.2))*255) : ((per <= .7) ? (255) : (per <=.9) ? (255-((per-.7)/(.2))*255) : (0))),
            (per <= .3) ? (255) : (per <= .5) ? (255-((per-.3)/(.2))*255) : (0)
        ));
    }

    //init static colours
    basicColours = new std::vector<const QColor*>();

    basicColours->push_back(new QColor (255, 255, 255));
    basicColours->push_back(new QColor(255, 255, 0));
    basicColours->push_back(new QColor(255, 0, 0));
    basicColours->push_back(new QColor(0, 255, 0));
    basicColours->push_back(new QColor(0, 0, 255));
    basicColours->push_back(new QColor (255, 132, 0));
    basicColours->push_back(new QColor(0, 190, 255));
    basicColours->push_back(new QColor (255, 0, 150));
    basicColours->push_back(new QColor(160, 0, 200));
    basicColours->push_back(new QColor(0, 0, 0));

    //set up self reference
    instance = this;
}

ResourceManager::~ResourceManager()
{
    //unload all textures
    while(!textures->empty()){
        glDeleteTextures(1, &textures->back());
        textures->pop_back();
    }
    delete textures;

    //remove colour scale
    while(!gradientColours->empty()) delete gradientColours->back(), gradientColours->pop_back();
    delete gradientColours;

    //delete static colours
    while(!basicColours->empty()) delete basicColours->back(), basicColours->pop_back();
    delete basicColours;
}

//loads an image from a file and returns the OpenGL handle for it
GLuint ResourceManager::loadTextureFromFile(const char* c){
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

//return a texture
GLuint ResourceManager::getTexture(Texture t){
    return textures->at(t);
}

//return the stored colour of the specified gradient
const QColor* ResourceManager::getColourScale(const float f){
    if (f < 0){
        return gradientColours->at(0);
    }
    else if (f > 1){
        return gradientColours->at(NUM_COLOURS - 1);
    }
    else{
        return gradientColours->at(round(f * (NUM_COLOURS - 1)));
    }
}

const QColor* ResourceManager::getColour(Colours c){
    return basicColours->at((int)c);
}
