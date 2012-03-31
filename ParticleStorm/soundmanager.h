#ifndef SOUNDMANAGER_H
#define SOUNDMANAGER_H

#include <QSound>
#include "vector"

class SoundManager{
public:
    SoundManager();
    ~SoundManager();

    enum Sound{
        GAME,
        TITLE,
        NONE
    };

    static SoundManager* getInstance(){return instance;}

    void update(double deltaTime);
    void playSound(Sound s);
    void stopSound(){playSound(NONE);}

private:
    //sound management
    std::vector<double> *sound_times;

    //holds the sounds
    std::vector<QSound*> *sounds;
    double currSoundTime;
    Sound currSound;

    //self reference
    static SoundManager *instance;
};

#endif // SOUNDMANAGER_H
