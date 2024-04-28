#include "SoundPlayer.h"

#include <system_error>
#include <iostream>



std::map<std::string, std::string> SoundPlayer::sounds;

void SoundPlayer::load(const std::string& filename, const std::string& id){
    sounds[id] = filename;
}

void SoundPlayer::play(const std::string& id, bool loop){
    BOOL result = PlaySoundA(
        sounds[id].c_str(), // pszSound
        nullptr, // hmod
        SND_FILENAME
        | SND_ASYNC
        | (loop ? SND_LOOP : 0)// fdwSound
    );
    if(!result){
        int error = GetLastError();
        std::cout << "Error playing sound: " << std::system_category().message(error) << std::endl;
    }
}

void SoundPlayer::stop(){
    PlaySoundA(
        nullptr, // pszSound
        nullptr, // hmod
        0 // fdwSound
    );
}
