#include <iostream>
#include <cmath>

#include "CheckBox.h"
#include "../gl_canvas2d.h"

void Checkbox::Style::setState(Checkbox::State state, Vector3 background, Vector3 border, Vector3 check){
    this->backgroundColor[state] = background;
    this->borderColor[state] = border;
    this->checkColor[state] = check;
}

void Checkbox::Style::getState(Checkbox::State state, Vector3 *background, Vector3 *border, Vector3 *check){
    *background = this->backgroundColor[state];
    *border = this->borderColor[state];
    *check = this->checkColor[state];
}

Checkbox::Style* Checkbox::Style::Windows10(){
    Checkbox::Style *stl = new Checkbox::Style;

    stl->foregroundColor = Vector3(0., 0., 0.);
    stl->setState(Checkbox::State::NORMAL, Vector3(1.000, 1.000, 1.000), Vector3(0.200, 0.200, 0.200), Vector3(0.200, 0.200, 0.200));
    stl->setState(Checkbox::State::HOVER,  Vector3(1.000, 1.000, 1.000), Vector3(0.000, 0.470, 0.843), Vector3(0.000, 0.470, 0.843));
    stl->setState(Checkbox::State::CLICK,  Vector3(0.800, 0.894, 0.968), Vector3(0.000, 0.329, 0.600), Vector3(0.000, 0.329, 0.600));
    return stl;
}

Checkbox::Style::~Style(){
    backgroundColor.clear();
    borderColor.clear();
    checkColor.clear();
    std::cout << "Deleting Checkbox Style" << std::endl;
}


Checkbox::Checkbox(Vector2 pos, Vector2 size, std::string label, bool defaultValue) {
    this->pos = pos;
    this->sz = size;

    this->value = defaultValue;
    this->label = label;
}

Checkbox::~Checkbox(){
    std::cout << "Deleting Checkbox" << std::endl;
}

void Checkbox::draw(){
    CV::translate(pos);

    // draw debug outline
    CV::color(1,0,0);
    CV::rect(Vector2::zero(), sz);

    // draw background

    // draw border

    // draw checkbox

    // draw label
}


Vector2 Checkbox::getPos(){
    return this->pos;
}

Vector2 Checkbox::getSize(){
    return this->sz;
}

bool Checkbox::getValue(){
    return this->value;
}

void Checkbox::setValue(bool value){
    this->value = value;
}
