#include "SideBar.h"
#include "../gl_canvas2d.h"
#include "../Image/Bmp.h"

SideBar::SideBar(Vector2 pos, Vector2 size, int *scrW, int *scrH)
    : pos(pos), size(size){
    this->uiManager = new UIManager();
    
}

void SideBar::submitButtons(){

}

void SideBar::loadBmp(int n){
    std::string path = "./T1-RodrigoAppelt/images/" + std::string(1,'a'+n) + ".bmp";
    std::cout << "Loading bitmap: " << path << std::endl;
    Bmp *bmp = new Bmp(path.c_str());

    Image *img = new Image(bmp->getWidth(), bmp->getHeight());
    memmove(img->pixels, bmp->getImage(), bmp->getWidth()*bmp->getHeight()*3);
    std::cout << "Moved data from bmp to image" << std::endl;
    std::cout << "Image size: " << img->width << "x" << img->height << "(" << bmp->getWidth()*bmp->getHeight()*3 << ")"<< std::endl;
    images[n] = img;
    delete bmp;
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