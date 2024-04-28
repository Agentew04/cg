#ifndef __SOUND_PLAYER_H__
#define __SOUND_PLAYER_H__

#include <string>
#include <map>
#include <windows.h>
#include <mmsystem.h>

/// @brief Usa a biblioteca windows.h para
/// tocar sons assincronos
class SoundPlayer{
public:

    static void load(const std::string& filename, const std::string& id);
    
    static void play(const std::string& id, bool loop = false);

    static void stop();

private:

    static std::map<std::string, std::string> sounds;
};

#endif