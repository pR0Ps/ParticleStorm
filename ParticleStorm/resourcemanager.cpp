#include "resourcemanager.h"
#include <QImage>
#include <QtOpenGL>
#include <QSound>

ResourceManager* ResourceManager::instance = NULL;

const int ResourceManager::NUM_GRADIENT_COLOURS = 10;

ResourceManager::ResourceManager()
{
    if(instance){
        qDebug() << "Only one instance of TextureManager allowed";
        return;
    }

    //load the textures
    textures = new std::vector<GLuint>;
    textures->push_back(Util::loadTextureFromFile(":/Images/font.png"));

    //init colour array
    gradientColours = new std::vector<const Util::Color*>;
    float per;
    for (int i = 0 ; i < NUM_GRADIENT_COLOURS ; i++){
        //(gradiant from blue -> green -> red, like UI mockup)
        per = i/(float)NUM_GRADIENT_COLOURS;
        //hard one-liner is hard
        //still a little buggy
        gradientColours->push_back(new Util::Color(
            (per <= .1) ? ((255/2.0)-(per/.1)*(255/2.0)) : ((per < .5) ? (0) : ((per <= .7) ? (((per-.5)/.2)*255) : (255))),
            (per <= .1) ? (0) : ((per <= .3) ? (((per-.1)/(.2))*255) : ((per <= .7) ? (255) : (per <=.9) ? (255-((per-.7)/(.2))*255) : (0))),
            (per <= .3) ? (255) : (per <= .5) ? (255-((per-.3)/(.2))*255) : (0)
        ));
    }

    //init static colours
    basicColours = new std::vector<const Util::Color*>();

    basicColours->push_back(new Util::Color (255, 255, 255));
    basicColours->push_back(new Util::Color(255, 255, 0));
    basicColours->push_back(new Util::Color(255, 0, 0));
    basicColours->push_back(new Util::Color(0, 255, 0));
    basicColours->push_back(new Util::Color(0, 0, 255));
    basicColours->push_back(new Util::Color (255, 132, 0));
    basicColours->push_back(new Util::Color(0, 190, 255));
    basicColours->push_back(new Util::Color (255, 0, 150));
    basicColours->push_back(new Util::Color(160, 0, 200));
    basicColours->push_back(new Util::Color(0, 0, 0));

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

//return a texture
GLuint ResourceManager::getTexture(Texture t){
    return textures->at(t);
}

//return the stored colour of the specified gradient
const Util::Color* ResourceManager::getColourScale(const float f){
    if (f < 0){
        return gradientColours->at(0);
    }
    else if (f > 1){
        return gradientColours->at(NUM_GRADIENT_COLOURS - 1);
    }
    else{
        return gradientColours->at(round(f * (NUM_GRADIENT_COLOURS - 1)));
    }
}

const Util::Color* ResourceManager::getColour(Colours c){
    return basicColours->at((int)c);
}
