#include "FontManager.h"

#include <map>

#include "../3D/ObjLoader.h"

std::map<FontName, FontManager::Font> FontManager::fonts;

void FontManager::load(const std::string& fontPath, FontName fontName){
    if (fonts.find(fontName) != fonts.end())
    {
        return;
    }

    // load glyphs into 3d engine
    ObjLoader::load(fontPath, fontPath);

    // create metrics for each glyph
    auto availableGlyphs = ObjLoader::getModelNames(fontPath);

    Font font;
    float averageHeight = 0;
    float averageWidth = 0;

    for (auto &glyph : availableGlyphs)
    {
        Model3D* model = ObjLoader::get(fontPath, glyph);
        font.glyphs[glyph[0]] = model;
        float maxX = 0, maxY = 0;
        for(auto &v: model->vertices){
            if(v.x > maxX){
                maxX = v.x;
            }
            if(v.y > maxY){
                maxY = v.y;
            }
        }
        averageHeight += maxY;
        averageWidth += maxX;

        font.metrics.glyphSize[glyph[0]] = Vector2(maxX, maxY);
        font.metrics.availableGlyphs.push_back(glyph[0]);

        #if TEXT_DEBUG
        std::cout << "(FontManager)[FONT=" << 
            std::to_string((int)fontName)
            << "] Loaded glyph '" << glyph[0] 
            << "' with size " << maxX << "x" << maxY << std::endl;
        #endif
    }

    font.metrics.lineHeight = (averageHeight / availableGlyphs.size()) * 1.3f;
    font.metrics.spaceSize = averageWidth / availableGlyphs.size();
    font.metrics.glyphSpacing = font.metrics.spaceSize / 10.0f;

    #if TEXT_DEBUG
    std::cout << "(FontManager)[FONT=" << 
        std::to_string((int)fontName)
        << "] Line height: " << font.metrics.lineHeight 
        << "; Space size: " << font.metrics.spaceSize
        << "; Glyph spacing: " << font.metrics.glyphSpacing << std::endl;
    #endif

    fonts[fontName] = font;
}


float FontManager::getLineHeight(FontName font, float pt)
{
    return fonts[font].metrics.lineHeight * pt;
}

float FontManager::getTextWidth(FontName font, const std::string &text, float pt){
    float maxWidth = 0;
    float width = 0;
    for (auto &c : text)
    {
        if(c == '\n'){
            width = 0;
            continue;
        }
        width += FontManager::getGlyphSize(font, c, pt).x;
        width += FontManager::getFontSpacing(font, pt);
        if(width > maxWidth){
            maxWidth = width;
        }
    }
    return maxWidth;
}

float FontManager::getTextHeight(FontName font, const std::string &text, float pt){
    // count how many '\n' are in the text
    int slashn = 0;
    float maxHeight = 0;
    for (auto &c : text)
    {
        if(c == '\n'){
            slashn++;
        }else{
            float height = FontManager::getGlyphSize(font, c, pt).y;
            if(height > maxHeight){
                maxHeight = height;
            }
        }
    }

    if(slashn > 0){
        return (slashn+1) * FontManager::getLineHeight(font, pt);
    }else{
        return maxHeight;
    }
}

Vector2 FontManager::getGlyphSize(FontName font, char c, float pt){
    if(c == ' '){
        return Vector2(fonts[font].metrics.spaceSize * pt, 0);
    }
    if(c == '\n'){
        return Vector2(0, fonts[font].metrics.lineHeight * pt);
    }
    return fonts[font].metrics.glyphSize[c].multiply(Vector2(pt));
}

Vector2 FontManager::getTextSize(FontName font, const std::string &text, float pt){
    return Vector2(
        FontManager::getTextWidth(font, text, pt),
        FontManager::getTextHeight(font, text, pt)
    );
}

Model3D* FontManager::getGlyph(FontName font, char c){
    return fonts[font].glyphs[c];
}

float FontManager::getFontSpacing(FontName font, float pt){
    return fonts[font].metrics.glyphSpacing * pt;
}

bool FontManager::isDefined(FontName font, char c){
    if(c==' ' || c=='\n'){
        return true;
    }
    return fonts[font].metrics.glyphSize.find(c) != fonts[font].metrics.glyphSize.end();
}
