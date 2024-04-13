#ifndef __FONT_MANAGER_H__
#define __FONT_MANAGER_H__

#include <string>
#include <vector>

#include "schrift.h"

class FontManager {
public:
    /// @brief Carrega uma fonte do disco.
    static void load(std::string fontPath);
    /// @brief Libera todas as fontes carregadas.
    static void free();
private:
    static std::vector<SFT_Font*> fonts;
};

#endif