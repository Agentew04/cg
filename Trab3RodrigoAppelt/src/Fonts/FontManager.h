#ifndef __FONT_MANAGER_H__
#define __FONT_MANAGER_H__

#include <string>
#include <vector>

#include "../3D/ObjLoader.h"

enum class CustomFont {
    AgencyFB_Digits
};

/// @brief Carrega e gerencia fontes customizadas
/// usando arquivos obj.
class FontManager {
public:
    /// @brief Carrega uma fonte do disco.
    static void load(CustomFont font);

    /// @brief Retorna a altura de uma linha de texto
    static void getLineHeight(CustomFont font, uint32_t &lineHeight);

    /// @brief Retorna as dimensoes de um caractere em uma fonte
    static void getGlyphSize(CustomFont font, char c, uint32_t &width, uint32_t &height);

    /// @brief Retorna a largura e altura de um texto
    /// @param font A fonte a ser usada
    /// @param text O texto a ser medido
    /// @param width A largura do texto
    /// @param height A altura do texto
    static void getTextSize(CustomFont font, const std::string &text, uint32_t &width, uint32_t &height);

    /// @brief Retorna o arquivo 3d correspondente do caractere 'c' na fonte 'font'
    /// @param font A fonte a ser usada
    /// @param c O caractere a ser buscado
    /// @return O arquivo 3d correspondente
    static ObjFile getGlyph(CustomFont font, char c);

private:
    static std::map<std::tuple<CustomFont, char>, std::tuple<float,float>> glyphSize;
    static std::map<std::tuple<CustomFont, char>, std::string> objNames;
};

#endif