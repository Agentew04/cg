#include <vector>
#include <iostream>

#include "CheckBoxManager.h"

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

void CheckboxManager::registerCheckbox(Checkbox *checkbox){
    this->checkboxes.push_back(checkbox);
}

void CheckboxManager::unregisterCheckbox(Checkbox *checkbox){
    for (auto it = this->checkboxes.begin(); it != this->checkboxes.end(); ++it) {
        if (*it == checkbox) {
            this->checkboxes.erase(it);
            break;
        }
    }
}

CheckboxManager::CheckboxManager(){

}

CheckboxManager::~CheckboxManager(){
    for(size_t i=0; i<this->checkboxes.size(); i++){
        delete this->checkboxes[i]->style;
        delete this->checkboxes[i];
    }
    std::cout << "Deleting Checkbox Manager" << std::endl;
}

void CheckboxManager::draw(){
    for(size_t i=0; i<this->checkboxes.size(); i++){
        this->checkboxes[i]->draw();
    }
}

void CheckboxManager::updateMousePos(Vector2 mousePos){
    this->mousePos = mousePos;

    for(size_t i=0; i<this->checkboxes.size(); i++){
        Checkbox *s = this->checkboxes[i];

        // no mouse
        if(inside(s->getPos(), s->getSize(), mousePos)){
            if(s->state != Checkbox::State::CLICK){
                s->state = Checkbox::State::HOVER;
            }
        }else{
            if(s->state != Checkbox::State::CLICK){
                s->state = Checkbox::State::NORMAL;
            }
        }
    }
}

void CheckboxManager::mouseDown(){
    for (auto it = this->checkboxes.begin(); it != this->checkboxes.end(); ++it) {
        Checkbox* c = *it;
        if(inside(c->getPos(), c->getSize(), mousePos)){
            this->holdingCheckbox = c;
            this->holdingCheckbox->state = Checkbox::State::CLICK;
            break;
        }
    }
}

void CheckboxManager::mouseUp(){
    if(this->holdingCheckbox == nullptr){
        return;
    }

    this->holdingCheckbox->state = Checkbox::State::NORMAL;
    this->holdingCheckbox->setValue(!this->holdingCheckbox->getValue());
    holdingCheckbox = nullptr;
}
