#include "SideBar.h"
#include "../gl_canvas2d.h"
#include "../Image/Bmp.h"

SideBar::SideBar(Vector2 pos, Vector2 size, int *scrW, int *scrH)
    : pos(pos), size(size){
    this->uiManager = new UIManager();

    submitButtons();
}

void SideBar::submitButtons(){
    ButtonStyle *btnstl = ButtonStyle::Windows10();
    Button *ldbtn1 = new Button(pos + Vector2(5,5), Vector2(100,50), "Load 1", [&, ldbtn1](){
        loadBmp(0);
        ldbtn1->clickable = false;
    });
    ldbtn1->style = btnstl;
    this->uiManager->add(ldbtn1);

    Button *ldbtn2 = new Button(pos + Vector2(110,5), Vector2(100,50), "Load 2", [&, ldbtn2](){
        loadBmp(1);
        ldbtn2->clickable = false;
    });
    ldbtn2->style = btnstl;
    this->uiManager->add(ldbtn2);

    Button *ldbtn3 = new Button(pos + Vector2(215,5), Vector2(100,50), "Load 3", [&, ldbtn3](){
        loadBmp(2);
        ldbtn3->clickable = false;
    });
    ldbtn3->style = btnstl;
    this->uiManager->add(ldbtn3);
}

void SideBar::loadBmp(int n){
    if(images.find(n) != images.end()){
        return;
    }
    std::string path = "./T1-RodrigoAppelt/images/" + std::string(1,'a'+n) + ".bmp";
    std::cout << "Loading bitmap: " << path << std::endl;
    Bmp *bmp = new Bmp(path.c_str());

    Image *img = new Image(bmp->getWidth(), bmp->getHeight());
    memmove(img->pixels, bmp->getImage(), bmp->getWidth()*bmp->getHeight()*3);
    std::cout << "Moved data from bmp to image" << std::endl;
    int w,h;
    img->getSize(&w, &h);
    std::cout << "Image size: " << w << "x" << h << "(" << bmp->getWidth()*bmp->getHeight()*3 << ")"<< std::endl;
    images[n] = img;
    delete bmp;
}

SideBar::~SideBar(){
    delete this->uiManager;
    for(auto img : images){
        delete img.second;
    }
}

void SideBar::draw(){
    CV::translate(pos);
    CV::color(0,1,0);
    CV::rectFill(Vector2::zero(), this->size);

    this->uiManager->draw();
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
