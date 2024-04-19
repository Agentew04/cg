#include "FontManager.h"

#include <map>
#include <tuple>    

#include "../3D/ObjLoader.h"


std::map<std::tuple<CustomFont, char>, std::tuple<float,float>> FontManager::glyphSize;
std::map<std::tuple<CustomFont, char>, std::string> FontManager::objNames;
std::map<CustomFont, bool> FontManager::loadedFonts;
std::map<std::tuple<CustomFont, char>, bool> FontManager::definedChars;

void FontManager::load(CustomFont font)
{
    if (loadedFonts[font])
    {
        return;
    }
    loadedFonts[font] = true;

    switch (font)
    {
    case CustomFont::AgencyFB_Digits:
        // load glyphs
        ObjLoader::load("./Trab3RodrigoAppelt/models/agency_digit0.obj", "agency_digit0");
        ObjLoader::load("./Trab3RodrigoAppelt/models/agency_digit1.obj", "agency_digit1");
        ObjLoader::load("./Trab3RodrigoAppelt/models/agency_digit2.obj", "agency_digit2");
        ObjLoader::load("./Trab3RodrigoAppelt/models/agency_digit3.obj", "agency_digit3");
        ObjLoader::load("./Trab3RodrigoAppelt/models/agency_digit4.obj", "agency_digit4");
        ObjLoader::load("./Trab3RodrigoAppelt/models/agency_digit5.obj", "agency_digit5");
        ObjLoader::load("./Trab3RodrigoAppelt/models/agency_digit6.obj", "agency_digit6");
        ObjLoader::load("./Trab3RodrigoAppelt/models/agency_digit7.obj", "agency_digit7");
        ObjLoader::load("./Trab3RodrigoAppelt/models/agency_digit8.obj", "agency_digit8");
        ObjLoader::load("./Trab3RodrigoAppelt/models/agency_digit9.obj", "agency_digit9");

        // calculate glyph sizes
        for (int i = 0; i < 10; i++)
        {
            char c = '0' + i;
            float width, height;

            // get max delta in Y of the '1' vertices
            objNames[std::make_tuple(font, c)] = "agency_digit" + std::to_string(i);
            ObjFile *obj = ObjLoader::get("agency_digit" + std::to_string(i));
            float minX = 0, maxX = 0, minY = 0, maxY = 0;
            for (auto &v : obj->vertices)
            {
                if (v.x < minX)
                {
                    minX = v.x;
                }
                if (v.x > maxX)
                {
                    maxX = v.x;
                }
                if (v.y < minY)
                {
                    minY = v.y;
                }
                if (v.y > maxY)
                {
                    maxY = v.y;
                }
            }
            width = maxX - minX;
            height = maxY - minY;
            glyphSize[std::make_tuple(font, c)] = std::make_tuple(width, height);
            definedChars[std::make_tuple(font, c)] = true;
        }
        
        // space glyph
        glyphSize[std::make_tuple(font, ' ')] = std::make_tuple(0.2f, 0.2f);
        definedChars[std::make_tuple(font, ' ')] = true;
        break;
    default:
        break;
    }
}

void FontManager::getLineHeight(CustomFont font, float &lineHeight)
{
    switch (font)
    {
    case CustomFont::AgencyFB_Digits:
        // read '1' height
        float width, height;
        FontManager::getGlyphSize(font, '1', width, height);
        lineHeight = height;
        break;
    default:
        break;
    }
}

void FontManager::getGlyphSize(CustomFont font, char c, float &width, float &height){
    switch (font)
    {
    case CustomFont::AgencyFB_Digits:
        std::tie(width, height) = glyphSize[std::make_tuple(font, c)];
        break;
    default:
        break;
    }

}

void FontManager::getTextSize(CustomFont font, const std::string &text, float &width, float &height){
    width = 0;
    height = 0;
    for (auto &c : text)
    {
        float glyphWidth, glyphHeight;
        FontManager::getGlyphSize(font, c, glyphWidth, glyphHeight);
        width += glyphWidth;
        if (glyphHeight > height)
        {
            height = glyphHeight;
        }
    }
}

ObjFile* FontManager::getGlyph(CustomFont font, char c){
    switch (font)
    {
    case CustomFont::AgencyFB_Digits:
        return ObjLoader::get(objNames[std::make_tuple(font, c)]);
        break;
    default:
        break;
    }
    std::cout << "Nao achei glifo '" << c << "' na fonte " << (int)font << std::endl;
    return nullptr;
}

float FontManager::getFontSpacing(CustomFont font){
    switch (font)
    {
    case CustomFont::AgencyFB_Digits:
        return 0.07f;
        break;
    default:
        break;
    }
    return 0;
}

bool FontManager::isDefined(CustomFont font, char c){
    if (definedChars.find(std::make_tuple(font, c)) == definedChars.end())
    {
        return false;
    }
    return definedChars[std::make_tuple(font, c)];
}