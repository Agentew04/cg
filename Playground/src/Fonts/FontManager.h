#ifndef __FONT_MANAGER_H__
#define __FONT_MANAGER_H__

#include <string>
#include <vector>

#include "../3D/ObjLoader.h"
#include "../Math/Vector2.h"

enum class FontName {
    JetBrainsMono
};

/// @brief Carrega e gerencia fontes customizadas
/// usando arquivos obj.
class FontManager {
public:
    /// @brief Carrega uma fonte do disco.
    /// @param font A fonte a ser carregada. Deve ser o
    /// caminho para um arquivo .font
    /// @param fontName O nome comum da fonte
    static void load(const std::string& fontPath, FontName fontName);


    /// @brief Retorna a altura de uma linha de texto
    static float getLineHeight(FontName font, float pt);

    /// @brief Retorna as dimensoes de um caractere em uma fonte
    static Vector2 getGlyphSize(FontName font, char c, float pt);

    /// @brief Retorna a largura e altura de um texto
    /// @param font A fonte a ser usada
    /// @param text O texto a ser medido
    /// @param pt O tamanho da fonte
    /// @return O tamanho do texto (largura, altura)
    static Vector2 getTextSize(FontName font, const std::string &text, float pt);

    /// @brief Retorna a largura de um texto
    /// @param font A fonte usada
    /// @param text O texto
    /// @param pt O tamanho da fonte
    /// @return A largura
    static float getTextWidth(FontName font, const std::string &text, float pt);

    /// @brief Retorna a altura de um texto
    /// @param font A fonte usada
    /// @param text O texto
    /// @param pt O tamanho da fonte
    /// @return A altura
    static float getTextHeight(FontName font, const std::string &text, float pt);

    /// @brief Retorna o arquivo 3d correspondente do caractere 'c' na fonte 'font'
    /// @param font A fonte a ser usada
    /// @param c O caractere a ser buscado
    /// @return O arquivo 3d correspondente
    static Model3D* getGlyph(FontName font, char c);

    /// @brief Retorna o espacamento entre caracteres
    static float getCharacterSpacing(FontName font, float pt);

    /// @brief Retorna se um caractere esta definido na fonte
    static bool isDefined(FontName font, char c);

private:
    
    struct Metrics {
        std::map<char, Vector2> glyphSize;
        float spaceSize;
        float lineHeight;
        float glyphSpacing;
        std::vector<char> availableGlyphs;
    };
    
    struct Font {
        Metrics metrics;
        std::map<char, Model3D*> glyphs;
    };

    static std::map<FontName, Font> fonts;
};

#endif