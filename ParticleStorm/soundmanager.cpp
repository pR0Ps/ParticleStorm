#include "soundmanager.h"
#include "QDebug"
#include "QDir"

SoundManager* SoundManager::instance = NULL;

SoundManager::SoundManager(){

    if(instance){
        qDebug() << "Only one instance of SoundManager allowed";
        return;
    }

    //define the sound lengths
    sound_times = new std::vector<double>();
    sound_times->push_back(109);
    sound_times->push_back(34);

    currSoundTime = 0;
    currSound = NONE;

    //load the music
    sounds = new std::vector<QSound*>;
    #ifndef Q_WS_X11
    sounds->push_back(new QSound("../ParticleStorm/Resources/chaosinvaders-loop.wav"));
    sounds->push_back(new QSound("../ParticleStorm/Resources/base-loop.wav"));
    #endif

    instance = this;
}

SoundManager::~SoundManager(){
    //unload sounds
    while(!sounds->empty()) delete sounds->back(), sounds->pop_back();
    delete sounds;
    delete sound_times;
}

void SoundManager::update(double deltaTime){
    #ifdef Q_WS_X11
    return;
    #endif
    //check if playing anything
    if (currSound == NONE) return;

    //playing current sound
    currSoundTime -=deltaTime;
    //loop it if it's done playing
    if (currSoundTime <= 0){
        sounds->at(currSound)->play();
        currSoundTime = sound_times->at(currSound);
    }
}

void SoundManager::playSound(Sound s){
    #ifdef Q_WS_X11
    return;
    #endif
    //turning sound off
    if (s == NONE && currSound != NONE){
        sounds->at(currSound)->stop();
        currSound = NONE;
        currSoundTime = 0;
    }
    else if (s != currSound){
        //changing sounds
        currSound = s;
        currSoundTime = sound_times->at(s);
        sounds->at(currSound)->play();
    }
}
