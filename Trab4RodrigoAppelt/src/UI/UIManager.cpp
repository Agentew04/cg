#include <vector>
#include <iostream>

#include "UIManager.h"

void UIManager::add(Button *button){
    this->buttonMngr.registerButton(button);
}

void UIManager::remove(Button *button){
    this->buttonMngr.unregisterButton(button);
}

void UIManager::add(Slider *slider){
    this->sliderMngr.registerSlider(slider);
}

void UIManager::remove(Slider *slider){
    this->sliderMngr.unregisterSlider(slider);
}

void UIManager::add(Checkbox *checkbox){
    this->checkboxMngr.registerCheckbox(checkbox);
}

void UIManager::remove(Checkbox *checkbox){
    this->checkboxMngr.unregisterCheckbox(checkbox);
}

// void UIManager::add(Chart *chart){
//     this->charts.push_back(chart);
// }

// void UIManager::remove(Chart *chart){
//     for(auto it = this->charts.begin(); it != this->charts.end(); it++){
//         if(*it == chart){
//             this->charts.erase(it);
//             return;
//         }
//     }
// }

//void UIManager::add(ImageRenderer *img){
//    this->images.push_back(img);
//}

//void UIManager::remove(ImageRenderer *img){
//    for(auto it = this->images.begin(); it != this->images.end(); it++){
//        if(*it == img){
//            this->images.erase(it);
//            return;
//        }
//    }
//}


UIManager::UIManager(){

}

UIManager::~UIManager(){
//     for(auto chart : this->charts){
//         delete chart;
//     }
}

void UIManager::draw(){
    this->buttonMngr.draw();
    this->sliderMngr.draw();
    this->checkboxMngr.draw();
    // for(auto chart : this->charts){
    //     chart->draw();
    // }
    //for(auto img : this->images){
        //img->draw();
    //}
}

void UIManager::updateMousePos(Vector2 mousePos){
    this->mousePos = mousePos;

    this->buttonMngr.updateMousePos(mousePos);
    this->sliderMngr.updateMousePos(mousePos);
    this->checkboxMngr.updateMousePos(mousePos);
}

void UIManager::mouseDown(){
    this->buttonMngr.mouseDown();
    this->sliderMngr.mouseDown();
    this->checkboxMngr.mouseDown();
}

void UIManager::mouseUp(){
    this->buttonMngr.mouseUp();
    this->sliderMngr.mouseUp();
    this->checkboxMngr.mouseUp();
}
