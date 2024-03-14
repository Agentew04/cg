#include <vector>
#include <iostream>

#include "ButtonManager.h"

void ButtonManager::registerButton(Button button){
    this->buttons.push_back(button);
}

void unregisterButton(Button button){

}

ButtonManager::ButtonManager(){

}

ButtonManager::~ButtonManager(){

}

void ButtonManager::draw(){
    for(int i=0; i<this->buttons.size(); i++){
        this->buttons[i].draw();
    }
}

void ButtonManager::updateMousePos(Vector2 mousePos){
    this->mousePos = mousePos;
}

void ButtonManager::mouseDown(){

}

void ButtonManager::mouseUp(){
    this->holdingButton->callback();

    holdingButton = nullptr;
}
void ButtonManager::click(){
    //for(int i=0; i<this->buttons.size; i++){
    //    if(!this->buttons[i].clickable){
    //        continue;
    //    }
    //}
}
