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
    button->state = ButtonState::NORMAL;
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
    ButtonStyle::startUsing();
}

ButtonManager::~ButtonManager(){
    for(size_t i=0; i<this->buttons.size(); i++){
        delete this->buttons[i];
    }
    this->buttons.clear();
    ButtonStyle::freeStyles();
}

void ButtonManager::draw(){
    for(size_t i=0; i<this->buttons.size(); i++){
        Button* b = this->buttons[i];
        b->render();
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
        if(b->inside(mousePos)){
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
        if(button->inside(mousePos)){
            holdingButton = button;
            holdingButton->state = ButtonState::CLICK;
            break;
        }
    }
}

void ButtonManager::mouseUp(){
    if(holdingButton == nullptr){
        return;
    }

    if(holdingButton->inside(mousePos)){
        holdingButton->call();
    }

    holdingButton->state = ButtonState::NORMAL;
    holdingButton = nullptr;
}
