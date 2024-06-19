#include <vector>
#include <iostream>
#include <algorithm>

#include "CheckBoxManager.h"
#include "CursorManager.h"

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
    this->holdingCheckbox = nullptr;
}

CheckboxManager::~CheckboxManager(){
    std::vector<Checkbox::Style*> deleted;
    for(size_t i=0; i<this->checkboxes.size(); i++){
        if (std::find(deleted.begin(), deleted.end(), this->checkboxes[i]->style) == deleted.end()) {
            deleted.push_back(this->checkboxes[i]->style);
            delete this->checkboxes[i]->style;
        }
        delete this->checkboxes[i];
    }
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
        if(s == nullptr){
            continue;
        }

        if(s->state == Checkbox::State::HOVER){
            CursorManager::setCursor(CursorManager::CursorType::CLICKABLE);
        }

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
