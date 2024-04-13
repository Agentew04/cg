#include "FontManager.h"
#include "schrift.h"

std::vector<SFT_Font*> FontManager::fonts;

void FontManager::load(std::string fontPath)
{
    SFT_Font *font = sft_loadfile(fontPath.c_str());
    if (font == NULL)
    {
        printf("FontManager::load::Erro ao carregar fonte %s\n", fontPath.c_str());
        return;
    }
    fonts.push_back(font);
}

void FontManager::free()
{
    for (SFT_Font *font : fonts)
    {
        sft_freefont(font);
    }
    fonts.clear();
}
