#include "soundmanager.h"
#include "QDebug"
#include "QDir"

SoundManager::SoundManager(){

    //defined sounds (ugly hackish code, look into this)
    const char* sound_files[] ={
        "../ParticleStorm/Resources/chaosinvaders.wav",
        "../ParticleStorm/Resources/base-loop.wav"
    };

    //yuck
    sound_times = new std::vector<double>();
    sound_times->push_back(109);
    sound_times->push_back(34);

    currSoundTime = 0;
    currSound = NONE;

    //load the music
    sounds = new std::vector<QSound*>;
    for (unsigned int i = 0; i<sizeof(sound_files)/sizeof(char*); i++){
        sounds->push_back(new QSound(sound_files[i]));
    }
}

SoundManager::~SoundManager(){
    //unload sounds
    while(!sounds->empty()) delete sounds->back(), sounds->pop_back();
    delete sounds;
    delete sound_times;
}

void SoundManager::update(double deltaTime){
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
