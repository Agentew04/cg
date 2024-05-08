#include "TextBox.h"

#include <iostream>
#include <vector>
#include <string>

#include "../gl_canvas2d.h"
#include "../Keyboard.h"

int TextBox::Style::styleObserverCount = 0;
TextBox::Style* TextBox::Style::flatWhiteStyle = nullptr;

void TextBox::Style::setState(UIState state, Vector3 background, Vector3 border){
    this->backgroundColor[state] = background;
    this->borderColor[state] = border;
}

void TextBox::Style::getState(UIState state, Vector3 *background, Vector3 *border){
    *background = this->backgroundColor[state];
    *border = this->borderColor[state];
}

TextBox::Style* TextBox::Style::FlatWhite(){
    if(flatWhiteStyle == nullptr){
        flatWhiteStyle = new TextBox::Style;
        flatWhiteStyle->foreground = Vector3::fromHex(0x000000);
        flatWhiteStyle->setState(UIState::NORMAL, Vector3::fromHex(0xFFFFFF), Vector3::fromHex(0xFFFFFF));
        flatWhiteStyle->setState(UIState::HOVER, Vector3::fromHex(0xE3E3E3), Vector3::fromHex(0xE3E3E3));
        flatWhiteStyle->setState(UIState::CLICK, Vector3::fromHex(0xABABAB), Vector3::fromHex(0xABABAB));
        flatWhiteStyle->setState(UIState::FOCUSED, Vector3::fromHex(0xFFFFFF), Vector3::fromHex(0x33BBFF));
    }
    return flatWhiteStyle;
}

void TextBox::Style::startUsing(){
    styleObserverCount++;
}

void TextBox::Style::freeStyles(){
    styleObserverCount--;
    if(styleObserverCount == 0){
        delete flatWhiteStyle;
    }
}

TextBox::TextBox(
    std::function<Vector2()> positionFunc,
    std::function<Vector2()> sizeFunc,
    UIPlacement placement,
    TextAlign textAlign) :
    mousePos(0,0),
    positionFunc(positionFunc),
    sizeFunc(sizeFunc),
    placement(placement),
    textAlign(textAlign),
    currentState(UIState::NORMAL),
    text("")
{
}

void TextBox::render(){
    Vector2 pos = positionFunc();
    Vector2 size = sizeFunc();
    translateCoordinates(pos, size, placement);

    CV::translate(pos);

    Vector3 background, border;
    Vector3 foreground = style->foreground;
    if(style == nullptr){
        return;
    }
    style->getState(currentState, &background, &border);

    CV::color(background);
    CV::rectFill(Vector2::zero(), size);

    CV::color(border);
    CV::rect(Vector2::zero(), size);

    CV::color(foreground);
    int margin = 5;
    CV::text(margin, (size.y/2) + CV::fontHeight(GLUT_BITMAP_TIMES_ROMAN_24)/2, 
        text.c_str(), 
        GLUT_BITMAP_TIMES_ROMAN_24, TextAlign::LEFT);

    // draw cursor
    float width = CV::textWidth(text.c_str(), GLUT_BITMAP_TIMES_ROMAN_24);
    CV::color(Vector3::fromHex(0x000000));
    CV::line(width + margin, margin, width + margin, size.y - margin, 3.0f);
}

void TextBox::keyDown(Key key){
    if(currentState != UIState::FOCUSED){
        return;
    }
    if(key == BACKSPACE){
        if(text.size() > 0){
            text.pop_back();
        }
    }else if(!iscntrl(key)){
        text.push_back((char)key);
    }
}

void TextBox::updateMousePos(Vector2 pos){
    mousePos = pos;
    Vector2 position = positionFunc();
    Vector2 size = sizeFunc();
    translateCoordinates(position, size, placement);

    if(currentState != UIState::FOCUSED && currentState != UIState::CLICK
        && inside(position, size, mousePos)){
        currentState = UIState::HOVER;
        return;
    }
    if(currentState == UIState::HOVER && !inside(position, size, mousePos)){
        currentState = UIState::NORMAL;
    }
}

void TextBox::mouseDown(){
    if(currentState == UIState::HOVER){
        currentState = UIState::CLICK;
        return;
    }

    Vector2 position = positionFunc();
    Vector2 size = sizeFunc();
    translateCoordinates(position, size, placement);
    if(currentState == UIState::FOCUSED && !inside(position, size, mousePos)){
        currentState = UIState::NORMAL;
    }
}

void TextBox::mouseUp(){
    if(currentState == UIState::CLICK){
        currentState = UIState::FOCUSED;
    }
}

bool TextBox::inside(Vector2 pos, Vector2 size, Vector2 mousePos){
    return (mousePos.x >= pos.x && mousePos.x <= pos.x + size.x) &&
           (mousePos.y >= pos.y && mousePos.y <= pos.y + size.y);
}

std::string TextBox::getText(){
    return text;
}