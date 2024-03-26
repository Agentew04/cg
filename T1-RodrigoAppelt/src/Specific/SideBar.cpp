#include <iterator>

#include "SideBar.h"
#include "../gl_canvas2d.h"
#include "../Image/Bmp.h"

SideBar::SideBar(Vector2 pos, Vector2 size, int *scrW, int *scrH)
    : pos(pos), size(size){
    this->uiManager = new UIManager();

    submitButtons();
    //submitHistogram();
    //submitImagePreview();
}



void SideBar::loadBmp(int n){
    if(loadedImages[n]){
        return;
    }

    // carrega o bitmap
    std::string path = "./T1-RodrigoAppelt/images/" + std::string(1,'a'+n) + ".bmp";
    Bmp *bmp = new Bmp(path.c_str());
    bmp->convertBGRtoRGB();

    // converte para classe Image
    Image *img = new Image(bmp->getWidth(), bmp->getHeight());
    memmove(img->pixels, bmp->getImage(), bmp->getWidth()*bmp->getHeight()*3);
    delete bmp;
    
    this->imgCanvas->submitImage(img);
}

SideBar::~SideBar(){
    delete this->uiManager;
}


void SideBar::draw(){
    CV::translate(pos);
    CV::color(0,1,0);
    CV::rectFill(Vector2::zero(), this->size);

    this->uiManager->draw();
}

void SideBar::updateMousePos(Vector2 mousePos){
    this->mousePos = mousePos;
    this->uiManager->updateMousePos(mousePos);
}

void SideBar::mouseDown(){
    this->uiManager->mouseDown();
}

void SideBar::mouseUp(){
    this->uiManager->mouseUp();
}
