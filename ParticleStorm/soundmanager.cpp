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

    const char* sound_files[] = {
        ":/Sounds/chaosinvaders-loop.wav",
        ":/Sounds/base-loop.wav"
    };
    for (unsigned int i = 0; i<sizeof(sound_files)/sizeof(char*); i++){
        sounds->push_back(new QSound(sound_files[i]));
    }

    instance = this;
}

SoundManager::~SoundManager(){
    //unload sounds
    while(!sounds->empty()){
        delete sounds->back();
        sounds->pop_back();
    }
    delete sounds;
}

void SoundManager::playSound(Sound s, int numLoops){
    //stop current sound
    if (currSound != NONE){
        sounds->at(currSound)->stop();
        currSound = NONE;
    }
    //play new sound
    if (s != NONE){
        currSound = s;
        sounds->at(currSound)->setLoops(numLoops);
        sounds->at(currSound)->play();
    }
}
