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

    //defined textures (KEEP IN SYNC WITH ENUM)
    const char* tex_files[] = {
        ":/Images/font.png",
        ":/Images/Player.png"
    };

    //defined sounds
    const char* music_sounds[] ={
       "../ParticleStorm/Resources/secondarysong.wav"
        "../ParticleStorm/Resources/base-loop.wav"
        "../ParticleStorm/Resources/zapping.wav"
    };

    //load the textures
    textures = new std::vector<GLuint>;
    for (unsigned int i = 0 ; i < sizeof(tex_files)/sizeof(char*) ; i++){
        textures->push_back(loadTextureFromFile(tex_files[i]));
    }

    //load the music
    sound = new std::vector<QSound*>;
    for (unsigned int i = 0; i<sizeof(music_sounds)/sizeof(char*); i++){
        //commented the line out because it doesn't do anything and causes a 5 sec pause when starting the game (for me anyway)
        //sound->push_back(new QSound(music_sounds[i]));
    }

    //init colour arrays
    colours = new std::vector<const QColor*>;
    float per;
    for (int i = 0 ; i < NUM_COLOURS ; i++){
        //(gradiant from blue -> green -> red, like UI mockup)
        per = i/(float)NUM_COLOURS;
        //hard one-liner is hard
        //still a little buggy
        colours->push_back(new QColor(
            (per <= .1) ? ((255/2.0)-(per/.1)*(255/2.0)) : ((per < .5) ? (0) : ((per <= .7) ? (((per-.5)/.2)*255) : (255))),
            (per <= .1) ? (0) : ((per <= .3) ? (((per-.1)/(.2))*255) : ((per <= .7) ? (255) : (per <=.9) ? (255-((per-.7)/(.2))*255) : (0))),
            (per <= .3) ? (255) : (per <= .5) ? (255-((per-.3)/(.2))*255) : (0)
        ));
    }

    //init static textures
    white = new QColor (255, 255, 255);
    yellow = new QColor(255, 255, 0);
    red = new QColor(255, 0, 0);
    green = new QColor(0, 255, 0);
    blue = new QColor(0, 0, 255);
    orange = new QColor (255, 132, 0);
    lightBlue = new QColor(0, 191, 255);
    lightRed = new QColor (255, 176, 176);
    black = new QColor(0, 0, 0);
    purple = new QColor(160, 0, 200);

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
    while(!colours->empty()) delete colours->back(), colours->pop_back();
    delete colours;

    //delete static colours
    delete white;
    delete yellow;
    delete red;
    delete green;
    delete blue;
    delete purple;
    delete black;
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

const QSound *ResourceManager::getSound(Sound t){
    return sound->at(t);
    }

//return the stored colour of the specified gradient
const QColor* ResourceManager::getColourScale(const float f){
    if (f < 0){
        return colours->at(0);
    }
    else if (f > 1){
        return colours->at(NUM_COLOURS - 1);
    }
    else{
        return colours->at(round(f * (NUM_COLOURS - 1)));
    }
}

const QColor* ResourceManager::getColour(Colours c){
    if (c == WHITE)
        return white;
    else if (c == YELLOW)
        return yellow;
    else if (c == RED)
        return red;
    else if (c == GREEN)
        return green;
    else if (c == BLUE)
        return blue;
    else if (c == ORANGE)
        return orange;
    else if (c == LIGHTBLUE)
        return lightBlue;
    else if (c == LIGHTRED)
        return lightRed;
    else if (c == PURPLE)
        return purple;
    else
        return black;
}


double musictime = ResourceManager::MAIN_LOOP_TIME;
void ResourceManager::playMainMusic(double deltaTime, bool turnOff){
    musictime = musictime+deltaTime;

    if (musictime > MAIN_LOOP_TIME && turnOff ==false ){
   QSound primarySong("../ParticleStorm/Resources/secondarysong.wav");
        //QSound primarySong(ResourceManager::getInstance()->getSound(music));//not sure why It is not working
         primarySong.play();
    musictime =0;
    }
    if (turnOff ==true){
        QSound primarySong("../ParticleStorm/Resources/secondarysong.wav");
        primarySong.stop();
        musictime=MAIN_LOOP_TIME;
    }
}

double musicTime2 =ResourceManager::GAME_LOOP_TIME;
void ResourceManager::playSecondMusic(double deltaTime, bool turnOff)
{
    musicTime2=musicTime2 +deltaTime;
    if(musicTime2 > GAME_LOOP_TIME && turnOff ==false){
        QSound secondarysong("../ParticleStorm/Resources/base-loop.wav");
        secondarysong.play();
        musicTime2=0;
    }
        if (turnOff ==true){
            QSound secondarysong("../ParticleStorm/Resources/base-loop.wav");
                    secondarysong.stop();
                    musicTime2 = GAME_LOOP_TIME;
        }
    }


double musicTime3 =4;

void ResourceManager::playZap(double deltaTime, bool turnOff){
    musicTime3 =musicTime3 +deltaTime;
    if(turnOff == false){
        QSound zapping("../ParticleStorm/Resources/zapping.wav");
        zapping.play();
        musicTime3 = 0;
    }
    if(turnOff ==true){
        QSound zapping("../ParticleStorm/Resources/zapping.wav");
        zapping.stop();
        musicTime3=4;
    }

}
