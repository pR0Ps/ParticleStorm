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

    void update(double deltaTime);
    void playSound(Sound s);
    void stopSound(){playSound(NONE);}

private:
    //sound management
    static const char* sound_files[];
    std::vector<double> *sound_times;

    //holds the sounds
    std::vector<QSound*> *sounds;
    double currSoundTime;
    Sound currSound;
};

#endif // SOUNDMANAGER_H
