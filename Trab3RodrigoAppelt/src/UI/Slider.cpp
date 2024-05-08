#include "Slider.h"
#include "../gl_canvas2d.h"

#include <iostream>
#include <cmath>

void Slider::Style::setState(Slider::State state, Vector3 handle){
    this->handleColor[state] = handle;
}

void Slider::Style::getState(Slider::State state, Vector3 *handle){
    *handle = this->handleColor[state];
}

Slider::Style* Slider::Style::Windows10(){
    Slider::Style *stl = new Slider::Style;

    stl->backSliderColor = Vector3(0.905, 0.917, 0.917);
    stl->outSliderColor = Vector3(0.839, 0.839, 0.839);
    stl->tickColor = Vector3(0.760, 0.760, 0.760);
    stl->setState(Slider::State::NORMAL, Vector3(0.000, 0.470, 0.843));
    stl->setState(Slider::State::HOVER, Vector3(0.090, 0.090, 0.090));
    stl->setState(Slider::State::CLICK, Vector3(0.800, 0.800, 0.800));
    return stl;
}

Slider::Style::~Style(){
    handleColor.clear();
}


Slider::Slider(Vector2 pos, Vector2 size, float minValue, float maxValue, float defaultValue, Slider::Orientation orientation) {
    this->value = defaultValue;
    this->minValue = minValue;
    this->maxValue = maxValue;

    // check if sizes match
    if(orientation == Slider::Orientation::VERTICAL){
        std::cout << "Vertical Slider is not supported yet, defaulting to Horizontal" << std::endl;
        orientation = Slider::Orientation::HORIZONTAL;
    }
    if(orientation == Slider::Orientation::HORIZONTAL){
        if(size.y > size.x){
            size = size.flipped();
        }
    }
    this->orientation = orientation;
    this->pos = pos;
    this->sz = size;
}

Slider::~Slider(){
    //this->callback = default;
}

void Slider::draw(){
    CV::translate(pos);

    // offsets declarations
    // retirados diretamente do Windows Forms
    // TODO mover para Style, para ser customizavel
    Vector2 backSliderMargin(0,6);
    float backSliderHeight = 4;
    Vector2 tickMargin(5,11);
    int lowTickHeight = 3;
    int highTickHeigth = 4;
    int numTicks = 10;

    // draw slider back rectangle
    CV::color(style->backSliderColor);
    CV::rectFill(backSliderMargin, Vector2(this->sz.x - backSliderMargin.x, backSliderMargin.y+backSliderHeight));
    CV::color(style->outSliderColor);
    CV::rect(backSliderMargin, Vector2(this->sz.x - backSliderMargin.x, backSliderMargin.y+backSliderHeight));

    // draw ticks
    float tickSpacing = (this->sz.x-(tickMargin.x*2))/(numTicks-1);
    CV::color(style->tickColor);
    for(int i=0; i<numTicks; i++){
        int x0 = backSliderMargin.x + tickMargin.x + tickSpacing*i;
        int y0 = backSliderMargin.y + tickMargin.y;
        int x1 = x0;
        int y1 = (i==0 || i==numTicks-1) ? y0+highTickHeigth : y0+lowTickHeight;
        CV::line(x0,y0,x1,y1);
    }

    // draw handle
    float vx[] = {0, 0,  5,  10, 10};
    float vy[] = {0, 13, 18, 13, 0};
    float handleX = pos.x + ((this->sz.x-(tickMargin.x*2))/(maxValue-minValue))*this->value;
    CV::translate(handleX , pos.y);
    CV::color(style->handleColor[this->state]);
    CV::polygonFill(vx,vy, sizeof(vx)/sizeof(float));
}


Vector2 Slider::getPos(){
    return this->pos;
}

Vector2 Slider::getSize(){
    return this->sz;
}

void Slider::getValueBounds(float *minValue, float *maxValue){
    *minValue = this->minValue;
    *maxValue = this->maxValue;
}

float Slider::getValue(){
    return this->value;
}

void Slider::setValue(float value, bool notify){
    // clamp
    if(value < this->minValue){
        value = this->minValue;
    }else if(value > this->maxValue){
        value = this->maxValue;
    }

    this->value = value;
    if(this->bindingTarget != nullptr){
        *this->bindingTarget = value;
    }
    if(hasCallback && notify){
        this->callback(value);
    }
}

void Slider::setBinding(float *target){
    this->bindingTarget = target;
}

void Slider::setCallback(std::function<void(float)> callback){
    hasCallback = true;
    this->callback = callback;
}
