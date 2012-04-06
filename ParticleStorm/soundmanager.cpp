#include "soundmanager.h"
#include "QDebug"
#include "QDir"

SoundManager* SoundManager::instance = NULL;

SoundManager::SoundManager(){

    if(instance){
        qDebug() << "Only one instance of SoundManager allowed";
        return;
    }

    currSound = NONE;

    //load the music
    sounds = new std::vector<QSound*>;

    #ifndef Q_WS_X11
    const char* sound_files[] = {
        "../ParticleStorm/Resources/chaosinvaders-loop.wav",
        "../ParticleStorm/Resources/base-loop.wav"
    };
    for (unsigned int i = 0; i<sizeof(sound_files)/sizeof(char*); i++){
        sounds->push_back(new QSound(sound_files[i]));
    }
    #endif

    instance = this;
}

SoundManager::~SoundManager(){
    //unload sounds
    while(!sounds->empty()) delete sounds->back(), sounds->pop_back();
    delete sounds;
}

void SoundManager::playSound(Sound s, int numLoops){
    #ifdef Q_WS_X11
    return;
    #endif
    //turning sound off
    if (s == NONE && currSound != NONE){
        sounds->at(currSound)->stop();
        currSound = NONE;
    }
    else if (s != currSound){
        //changing sounds
        currSound = s;
        sounds->at(currSound)->setLoops(numLoops);
        sounds->at(currSound)->play();
    }
}
