#include "SideBar.h"
#include "../gl_canvas2d.h"

SideBar::SideBar(Vector2 pos, Vector2 size, int *scrW, int *scrH)
    : pos(pos), size(size){
    this->uiManager = new UIManager();
    
}

SideBar::~SideBar(){
    delete this->uiManager;
}

void SideBar::draw(){
    this->uiManager->draw();
    CV::translate(pos);
    CV::color(0,1,0);
    CV::rectFill(Vector2::zero(), this->size);
    std::cout << pos << " " << size << std::endl;
}

void SideBar::updateMousePos(Vector2 mousePos){
    this->uiManager->updateMousePos(mousePos);
}

void SideBar::mouseDown(){
    this->uiManager->mouseDown();
}

void SideBar::mouseUp(){
    this->uiManager->mouseUp();
}