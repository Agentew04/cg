#ifndef __SOUND_PLAYER_H__
#define __SOUND_PLAYER_H__

#include <string>
#include <map>
#include <windows.h>
#include <mmsystem.h>

/// @brief Usa a biblioteca windows.h para
/// tocar sons de modo assincrono.
/// @remarks Consegue tocar apenas um audio
/// por vez. Para tocar mais de um audio ao
/// mesmo tempo, portar o codigo para usar
/// Direct Audio com win32! Se quiser desabilitar
/// a funcionalidade por completo, descomente a
/// linha #define DISABLE_SOUND no arquivo SoundPlayer.cpp
class SoundPlayer{
public:

    /// @brief Carrega um arquivo .wav na memoria
    /// @param filename O caminho do arquivo
    /// @param id O id do som para ser referenciado depois
    static void load(const std::string& filename, const std::string& id);
    
    /// @brief Toca um som no computador. So consegue
    /// tocar um som por vez.
    /// @param id O id do som a ser tocado
    /// @param loop Se o som deve ser tocado em loop ou nao
    static void play(const std::string& id, bool loop = false);

    /// @brief Para qualquer som sendo tocado
    static void stop();

private:

    static std::map<std::string, std::string> sounds;
};

#endif