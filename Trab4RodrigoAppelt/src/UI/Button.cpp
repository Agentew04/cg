#include "Button.h"

#include <iostream>

#include "../gl_canvas2d.h"
#include "UIDefinitions.h"

int ButtonStyle::styleObserverCount = 0;
ButtonStyle* ButtonStyle::windows10Style = nullptr;
ButtonStyle* ButtonStyle::flatLightBlueStyle = nullptr;
ButtonStyle* ButtonStyle::flatDarkBlueStyle = nullptr;
ButtonStyle* ButtonStyle::flatWhiteStyle = nullptr;
ButtonStyle* ButtonStyle::flatGreenStyle = nullptr;
ButtonStyle* ButtonStyle::flatRedStyle = nullptr;

void ButtonStyle::setState(ButtonState state, Vector3 background, Vector3 border){
    this->backgroundColor[state] = background;
    this->borderColor[state] = border;
}

void ButtonStyle::getState(ButtonState state, Vector3 *background, Vector3 *border){
    *background = this->backgroundColor[state];
    *border = this->borderColor[state];
}

ButtonStyle* ButtonStyle::Windows10(){
    if(windows10Style == nullptr){
        windows10Style = new ButtonStyle;
        windows10Style->foreground = Vector3::fromHex(0x000000);
        windows10Style->setState(ButtonState::NORMAL, Vector3(0.882,0.882,0.882), Vector3(0.678,0.678,0.678));
        windows10Style->setState(ButtonState::HOVER, Vector3(0.898, 0.945, 0.984), Vector3(0.000, 0.470, 0.843));
        windows10Style->setState(ButtonState::CLICK, Vector3(0.800, 0.894, 0.968), Vector3(0.000, 0.329, 0.600));   
    }
    return windows10Style;
}

ButtonStyle* ButtonStyle::FlatLightBlue(){
    if(flatLightBlueStyle == nullptr){
        flatLightBlueStyle = new ButtonStyle;
        flatLightBlueStyle->foreground = Vector3::fromHex(0xFFFFFF);
        flatLightBlueStyle->setState(ButtonState::NORMAL, Vector3::fromHex(0x28ABE3), Vector3::fromHex(0x28ABE3));
        flatLightBlueStyle->setState(ButtonState::HOVER, Vector3::fromHex(0x1C7BA0), Vector3::fromHex(0x1C7BA0));
        flatLightBlueStyle->setState(ButtonState::CLICK, Vector3::fromHex(0x13536B), Vector3::fromHex(0x13536B));
    }
    return flatLightBlueStyle;
}

ButtonStyle* ButtonStyle::FlatDarkBlue(){
    if(flatDarkBlueStyle == nullptr){
        flatDarkBlueStyle = new ButtonStyle;
        flatDarkBlueStyle->foreground = Vector3::fromHex(0xFFFFFF);
        flatDarkBlueStyle->setState(ButtonState::NORMAL, Vector3::fromHex(0x3D66FE), Vector3::fromHex(0x3D66FE));
        flatDarkBlueStyle->setState(ButtonState::HOVER, Vector3::fromHex(0x385CE0), Vector3::fromHex(0x385CE0));
        flatDarkBlueStyle->setState(ButtonState::CLICK, Vector3::fromHex(0x2D4BB7), Vector3::fromHex(0x2D4BB7));
    }
    return flatDarkBlueStyle;
}

ButtonStyle* ButtonStyle::FlatWhite(){
    if(flatWhiteStyle == nullptr){
        flatWhiteStyle = new ButtonStyle;
        flatWhiteStyle->foreground = Vector3::fromHex(0x000000);
        flatWhiteStyle->setState(ButtonState::NORMAL, Vector3::fromHex(0xFFFFFF), Vector3::fromHex(0xFFFFFF));
        flatWhiteStyle->setState(ButtonState::HOVER, Vector3::fromHex(0xC6C6C6), Vector3::fromHex(0xC6C6C6));
        flatWhiteStyle->setState(ButtonState::CLICK, Vector3::fromHex(0x969696), Vector3::fromHex(0x969696));
    }
    return flatWhiteStyle;
}

ButtonStyle* ButtonStyle::FlatGreen(){
    if(flatGreenStyle == nullptr){
        flatGreenStyle = new ButtonStyle;
        flatGreenStyle->foreground = Vector3::fromHex(0xFFFFFF);
        flatGreenStyle->setState(ButtonState::NORMAL, Vector3::fromHex(0x1EAA79), Vector3::fromHex(0x1EAA79));
        flatGreenStyle->setState(ButtonState::HOVER, Vector3::fromHex(0x18895E), Vector3::fromHex(0x18895E));
        flatGreenStyle->setState(ButtonState::CLICK, Vector3::fromHex(0x116042), Vector3::fromHex(0x116042));
    }
    return flatGreenStyle;
}

ButtonStyle* ButtonStyle::FlatRed(){
    if(flatRedStyle == nullptr){
        flatRedStyle = new ButtonStyle;
        flatRedStyle->foreground = Vector3::fromHex(0xFFFFFF);
        flatRedStyle->setState(ButtonState::NORMAL, Vector3::fromHex(0xD41E59), Vector3::fromHex(0xD41E59));
        flatRedStyle->setState(ButtonState::HOVER, Vector3::fromHex(0x991642), Vector3::fromHex(0x991642));
        flatRedStyle->setState(ButtonState::CLICK, Vector3::fromHex(0x630E2C), Vector3::fromHex(0x630E2C));
    }
    return flatRedStyle;
}

void ButtonStyle::startUsing(){
    styleObserverCount++;
}

void ButtonStyle::freeStyles(){
    if(styleObserverCount > 0){
        styleObserverCount--;
    }
    if(styleObserverCount > 0){
        return;
    }
    if(windows10Style != nullptr){
        delete windows10Style;
        windows10Style = nullptr;
    }
    if(flatLightBlueStyle != nullptr){
        delete flatLightBlueStyle;
        flatLightBlueStyle = nullptr;
    }
    if(flatDarkBlueStyle != nullptr){
        delete flatDarkBlueStyle;
        flatDarkBlueStyle = nullptr;
    }
    if(flatWhiteStyle != nullptr){
        delete flatWhiteStyle;
        flatWhiteStyle = nullptr;
    }
    if(flatGreenStyle != nullptr){
        delete flatGreenStyle;
        flatGreenStyle = nullptr;
    }
    if(flatRedStyle != nullptr){
        delete flatRedStyle;
        flatRedStyle = nullptr;
    }
}


Button::Button(std::function<Vector2()> positionFunc,
        std::function<Vector2()> sizeFunc, 
        UIPlacement placement,
        std::string text, 
        std::function<void(Button*)> callback) :
        positionFunc(positionFunc), 
        sizeFunc(sizeFunc),
        placement(placement),
        text(text),
        callback(callback){
    this->clickable = true;
}

Button::~Button() {
}

void Button::render(){
    Vector2 pos = positionFunc();
    Vector2 sz = sizeFunc();
    translateCoordinates(pos,sz,placement);
    CV::translate(pos);

    // draw background
    CV::color(style->backgroundColor[state]);
    CV::rectFill(Vector2::zero(), sz);

    // draw border
    CV::color(style->borderColor[state]);
    CV::rect(Vector2::zero(), sz);

    // draw text
    CV::color(style->foreground);
    CV::text(sz*0.5f, text, findPt());
    // float lineHeightRatio = 0.65;
    // CV::text(sz.x/2,
    //          (sz.y/2)+(glutBitmapHeight(GLUT_BITMAP_HELVETICA_18)*lineHeightRatio/2), text.c_str(), GLUT_BITMAP_HELVETICA_18, TextAlign::CENTER);
}

void Button::call(){
    if(this->clickable){
        this->callback(this);
    }
}

bool Button::inside(Vector2 mousePos){
    Vector2 pos = positionFunc();
    Vector2 sz = sizeFunc();
    translateCoordinates(pos,sz,placement);
    return (mousePos.x >= pos.x && mousePos.x <= pos.x + sz.x) &&
           (mousePos.y >= pos.y && mousePos.y <= pos.y + sz.y);
}

float Button::findPt(){
    int iterations = 10;
    float pt = 25; // initial try
    Vector2 maxSize = sizeFunc();
    for(int i=0; i<iterations; i++){
        Vector2 textSize = FontManager::getTextSize(FontName::JetBrainsMono, this->text, pt);

        if(textSize.x < maxSize.x && textSize.y < maxSize.y){
            // pode aumentar
            pt *= 1.25f;
        }else {
            // passou, diminui
            pt *= 0.75f;
        }
    }
    std::cout << std::endl;
    return pt;
}