#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <QSound>
#include "vector"

class SoundManager{
public:
    SoundManager();
    ~SoundManager();

    //make sure this matches with the sound_files array in the constructor
    enum Sound{
        GAME,
        TITLE,
        NONE //must be last
    };

    static SoundManager* getInstance(){return instance;}

    void playSound(Sound s, int numLoops = -1);
    void stopSound(){playSound(NONE);}

private:
    //holds the sounds
    std::vector<QSound*> *sounds;
    Sound currSound;

    //self reference
    static SoundManager *instance;
};

#endif // SOUNDMANAGER_H
