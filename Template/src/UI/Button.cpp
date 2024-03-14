#include "Button.h"
#include "../gl_canvas2d.h"

Button::Button(Vector2 pos, Vector2 sz, std::string text, bool relative, std::function<void()> callback) {
    this->text = text;
    this->pos = pos;
    this->sz = sz;
    this->callback = callback;
}

Button::~Button() {

}

void Button::draw(){

    CV::translate(pos);

    // draw background
    CV::color(background);
    CV::rectFill(pos, pos+sz);

    // draw border
    CV::color(borderColor);
    CV::rect(pos, pos+sz);

    // draw text
    CV::color(foreground);
    CV::text(30, 30, text.c_str());
}

void Button::call(){
    this->callback();
}
