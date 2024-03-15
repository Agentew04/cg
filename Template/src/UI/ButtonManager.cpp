#include <vector>
#include <iostream>

#include "ButtonManager.h"

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
    int idx = -1;
    for(int i=0; i<this->buttons.size(); i++){
        if(this->buttons[i] == button){
            idx = i;
            break;
        }
    }
    this->buttons.erase(idx);
}

ButtonManager::ButtonManager(){

}

ButtonManager::~ButtonManager(){
    for(int i=0; i<this->buttons.size(); i++){
        delete this->buttons[i];
    }
}

void ButtonManager::draw(){
    for(int i=0; i<this->buttons.size(); i++){
        this->buttons[i]->draw();
    }
}

void ButtonManager::updateMousePos(Vector2 mousePos){
    this->mousePos = mousePos;

    // foreach botao
    // se esta dentro
        // se estado n e click
            // muda pra hover
    // senao normal
    for(int i=0; i<this->buttons.size(); i++){
        Button *b = this->buttons[i];
        if(inside(b->getPos(), b->getSize(), mousePos)){
            if(b->state != ButtonState::CLICK){
                printf("Hover\n");
                b->state = ButtonState::HOVER;
            }
        }else{
            printf("Normal\n");
            b->state = ButtonState::NORMAL;
        }
    }
}

void ButtonManager::mouseDown(){

}

void ButtonManager::mouseUp(){
    this->holdingButton->call();

    holdingButton = nullptr;
}
