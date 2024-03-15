#include "Button.h"
#include "../gl_canvas2d.h"

#include <iostream>

void ButtonStyle::setState(ButtonState state, Vector3 background, Vector3 border){
    std::cout << "setting back to: " << background << std::endl;
    this->backgroundColor[state] = background;
    this->borderColor[state] = border;
}

void ButtonStyle::getState(ButtonState state, Vector3 *background, Vector3 *border){
    *background = this->backgroundColor[state];
    *border = this->borderColor[state];
}

ButtonStyle* ButtonStyle::Windows10(){
    ButtonStyle stl = new ButtonStyle;
    stl.foreground = Vector3(0,0,0);
    stl.setState(ButtonState::NORMAL, Vector3(0.882,0.882,0.882), Vector3(0.678,0.678,0.678));
    stl.setState(ButtonState::HOVER, Vector3(0.898, 0.945, 0.984), Vector3(0.000, 0.470, 0.843));
    stl.setState(ButtonState::CLICK, Vector3(0.800, 0.894, 0.968), Vector3(0.000, 0.329, 0.600));
    return stl;
}

ButtonStyle::~ButtonStyle(){
    backgroundColor.clear();
    borderColor.clear();
}

Button::Button(Vector2 pos, Vector2 sz, std::string text, bool relative, std::function<void()> callback) {
    this->text = text;
    this->pos = pos;
    this->sz = sz;
    this->callback = callback;
}

Button::~Button() {
    delete this->style;
}

void Button::draw(){

    CV::translate(pos);

    // draw background
    std::cout << "Background: " << style.backgroundColor[state] << std::endl;
    CV::color(style.backgroundColor[state]);
    CV::rectFill(Vector2::zero(), sz);

    // draw border
    CV::color(style.borderColor[state]);
    CV::rect(Vector2::zero(), sz);

    // draw text
    CV::color(style.foreground);
    float lineHeightRatio = 0.65;
    CV::text(sz.x/2,
             (sz.y/2)+(glutBitmapHeight(GLUT_BITMAP_HELVETICA_18)*lineHeightRatio/2), text.c_str(), GLUT_BITMAP_HELVETICA_18, TextAlign::CENTER);
}

void Button::call(){
    this->callback();
}

Vector2 Button::getPos(){
    return this->pos;
}

Vector2 Button::getSize(){
    return this->sz;
}
