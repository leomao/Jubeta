#ifndef JB_SOUND_H
#define JB_SOUND_H

#include <jb/jb.h>
#include <jb/string.h>


class jb::Sound
{
public:
    Sound();
    Sound(const String& filename);
    ~Sound();
    bool load(const String& filename);
    bool isOk();
    bool setOffset(int position);
    int getOffset();
    bool play();
    bool stop();
    bool toggle();

private:

};

#endif // JB_SOUND_H
