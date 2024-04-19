#include "FontManager.h"

#include <map>
#include <tuple>    

#include "../3D/ObjLoader.h"


std::map<std::tuple<CustomFont, char>, std::tuple<float,float>> FontManager::glyphSize;
std::map<std::tuple<CustomFont, char>, std::string> FontManager::objNames;

void FontManager::load(CustomFont font)
{
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
            uint32_t width, height;

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
        }
        break;
    default:
        break;
    }
}

void FontManager::getLineHeight(CustomFont font, uint32_t &lineHeight)
{
    switch (font)
    {
    case CustomFont::AgencyFB_Digits:
        // read '1' height
        uint32_t width, height;
        FontManager::getGlyphSize(font, '1', width, height);
        lineHeight = height;
        break;
    default:
        break;
    }
}

void FontManager::getGlyphSize(CustomFont font, char c, uint32_t &width, uint32_t &height){
    switch (font)
    {
    case CustomFont::AgencyFB_Digits:
        std::tie(width, height) = glyphSize[std::make_tuple(font, c)];
        break;
    default:
        break;
    }

}

void FontManager::getTextSize(CustomFont font, const std::string &text, uint32_t &width, uint32_t &height){
    width = 0;
    height = 0;
    for (auto &c : text)
    {
        uint32_t glyphWidth, glyphHeight;
        FontManager::getGlyphSize(font, c, glyphWidth, glyphHeight);
        width += glyphWidth;
        if (glyphHeight > height)
        {
            height = glyphHeight;
        }
    }
}

ObjFile FontManager::getGlyph(CustomFont font, char c){
    switch (font)
    {
    case CustomFont::AgencyFB_Digits:
        return *ObjLoader::get(objNames[std::make_tuple(font, c)]);
        break;
    default:
        break;
    }
    return ObjFile();
}