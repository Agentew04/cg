#include <vector>
#include <iostream>
#include <algorithm>

#include "ButtonManager.h"
#include "CursorManager.h"

bool inside(Vector2 buttonPos, Vector2 buttonSize, Vector2 mousePos){
    if(!(mousePos.x >= buttonPos.x && mousePos.x <= buttonPos.x + buttonSize.x)){
        return false;
    }
    return (mousePos.y >= buttonPos.y && mousePos.y <= buttonPos.y + buttonSize.y);
}

void ButtonManager::registerButton(Button *button){
    this->buttons.push_back(button);
}

void ButtonManager::unregisterButton(Button *button){
    for (auto it = this->buttons.begin(); it != this->buttons.end(); ++it) {
        if (*it == button) {
            this->buttons.erase(it);
            break;
        }
    }
}


ButtonManager::ButtonManager(){
    this->holdingButton = nullptr;
}

ButtonManager::~ButtonManager(){
    std::vector<ButtonStyle*> deleted;
    for(size_t i=0; i<this->buttons.size(); i++){
        if (std::find(deleted.begin(), deleted.end(), this->buttons[i]->style) == deleted.end()) {
            deleted.push_back(this->buttons[i]->style);
            delete this->buttons[i]->style;
        }
            
        delete this->buttons[i];
    }
    std::cout << "Deleting Button Manager" << std::endl;
}

void ButtonManager::draw(){
    for(size_t i=0; i<this->buttons.size(); i++){
        Button* b = this->buttons[i];
        b->draw();
    }
}

void ButtonManager::updateMousePos(Vector2 mousePos){
    this->mousePos = mousePos;

    for(size_t i=0; i<this->buttons.size(); i++){
        Button *b = this->buttons[i];
        if(b == nullptr){
            continue;
        }

        if(b->state == ButtonState::HOVER){
            CursorManager::setCursor(CursorManager::CursorType::CLICKABLE);
        }

        // no mouse
        if(inside(b->getPos(), b->getSize(), mousePos)){
            if(b->state != ButtonState::CLICK){
                b->state = ButtonState::HOVER;
            }
        }else{
            if(b->state != ButtonState::CLICK){
                b->state = ButtonState::NORMAL;
            }
        }
    }
}

void ButtonManager::mouseDown(){
    for (auto it = this->buttons.begin(); it != this->buttons.end(); ++it) {
        Button* button = *it;
        if(inside(button->getPos(), button->getSize(), mousePos)){
            this->holdingButton = button;
            this->holdingButton->state = ButtonState::CLICK;
            break;
        }
    }
}

void ButtonManager::mouseUp(){
    if(this->holdingButton == nullptr){
        return;
    }
    if(inside(this->holdingButton->getPos(), this->holdingButton->getSize(), mousePos)){
        this->holdingButton->call();
    }

    this->holdingButton->state = ButtonState::NORMAL;
    holdingButton = nullptr;
}
