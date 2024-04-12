#include <vector>
#include <iostream>
#include <algorithm>

#include "SliderManager.h"

static float clamp(float value, float minValue, float maxValue){
    if(value >= maxValue){
        return maxValue;
    }
    if(value <= minValue){
        return minValue;
    }
    return value;
}

static bool inside(Vector2 buttonPos, Vector2 buttonSize, Vector2 mousePos){
    if(!(mousePos.x >= buttonPos.x && mousePos.x <= buttonPos.x + buttonSize.x)){
        return false;
    }
    return (mousePos.y >= buttonPos.y && mousePos.y <= buttonPos.y + buttonSize.y);
}

void SliderManager::registerSlider(Slider *slider){
    this->sliders.push_back(slider);
}

void SliderManager::unregisterSlider(Slider *slider){
    for (auto it = this->sliders.begin(); it != this->sliders.end(); ++it) {
        if (*it == slider) {
            this->sliders.erase(it);
            break;
        }
    }
}

SliderManager::SliderManager(){
    this->holdingSlider = nullptr;
}

SliderManager::~SliderManager(){
    std::vector<Slider::Style*> deleted;
    for(size_t i=0; i<this->sliders.size(); i++){
        if (std::find(deleted.begin(), deleted.end(), this->sliders[i]->style) == deleted.end()) {
            deleted.push_back(this->sliders[i]->style);
            delete this->sliders[i]->style;
        }

        delete this->sliders[i];
    }
    std::cout << "Deleting Slider Manager" << std::endl;
}

void SliderManager::draw(){
    for(size_t i=0; i<this->sliders.size(); i++){
        this->sliders[i]->draw();
    }
}

void SliderManager::updateMousePos(Vector2 mousePos){
    this->mousePos = mousePos;

    for(size_t i=0; i<this->sliders.size(); i++){
        Slider *s = this->sliders[i];
        if(s == nullptr){
            continue;
        }

        // no mouse
        if(inside(s->getPos(), s->getSize(), mousePos)){
            if(s->state != Slider::State::CLICK){
                s->state = Slider::State::HOVER;
            }
        }else{
            if(s->state != Slider::State::CLICK){
                s->state = Slider::State::NORMAL;
            }
        }
    }

    // update value of currently holded slider
    if(this->holdingSlider != nullptr){
        Slider* s = this->holdingSlider;
        float localX = clamp(mousePos.x - s->getPos().x, 0, s->getSize().x-10);
        float minv,maxv;
        s->getValueBounds(&minv, &maxv);
        float delta = maxv-minv;
        float rate = delta / (s->getSize().x-10);
        s->setValue(localX*rate);
    }
}

void SliderManager::mouseDown(){
    for (auto it = this->sliders.begin(); it != this->sliders.end(); ++it) {
        Slider* s = *it;
        if(inside(s->getPos(), s->getSize(), mousePos)){
            this->holdingSlider = s;
            this->holdingSlider->state = Slider::State::CLICK;
            break;
        }
    }
}

void SliderManager::mouseUp(){
    if(this->holdingSlider == nullptr){
        return;
    }

    this->holdingSlider->state = Slider::State::NORMAL;
    holdingSlider = nullptr;
}
