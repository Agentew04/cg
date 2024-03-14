#include "Button.h"
#include "../gl_canvas2d.h"

Button::Button(Vector3 pos, Vector3 sz, std::string text, bool relative) {
    this->text = text;
    this->pos = pos;
    this->sz = sz;
}

Button::~Button() {

}

void Button::draw(){

    // draw background
    CV::color(background);
    CV::rectFill(pos, pos+sz);

    // draw border
    CV::color(borderColor);
    CV::rect(pos, pos+sz);

    // draw text
    CV::color(foreground);
    CV::text(30, 30, text);
}
