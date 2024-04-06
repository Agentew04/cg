#include <iterator>

#include "SideBar.h"
#include "../gl_canvas2d.h"
#include "../Image/Bmp.h"
#include "../Storage/PersistentStorage.h"

SideBar::SideBar(Vector2 pos, Vector2 size, int *scrW, int *scrH)
    : pos(pos), size(size){
    this->uiManager = new UIManager();


    submitButtons();
    submitHistogram();
}

void SideBar::load(){
    int tmp;
    PersistentStorage::setIfNotInt("sidebar", "loadedbmp0", 0);
    PersistentStorage::getInt("sidebar", "loadedbmp0", &tmp);
    if(tmp){
        loadBmp(0);
    }
    PersistentStorage::setIfNotInt("sidebar", "loadedbmp1", 0);
    PersistentStorage::getInt("sidebar", "loadedbmp1", &tmp);
    if(tmp){
        loadBmp(1);
    }
    PersistentStorage::setIfNotInt("sidebar", "loadedbmp2", 0);
    PersistentStorage::getInt("sidebar", "loadedbmp2", &tmp);
    if(tmp){
        loadBmp(2);
    }

    imgCanvas->readLocalSelected();

    ImageCanvas::Operation lastOp = ImageCanvas::Operation::NONE;
    PersistentStorage::getInt("sidebar", "lastOp", (int*)&lastOp);
    switch (lastOp)
    {
    case ImageCanvas::Operation::EXTRACT_R:
        imgCanvas->requestChannel(ImageManipulation::Channel::RED, false);
        break;
    case ImageCanvas::Operation::EXTRACT_G:
        imgCanvas->requestChannel(ImageManipulation::Channel::GREEN, false);
        break;
    case ImageCanvas::Operation::EXTRACT_B:
        imgCanvas->requestChannel(ImageManipulation::Channel::BLUE, false);
        break;
    case ImageCanvas::Operation::EXTRACT_Y:
        imgCanvas->requestChannel(ImageManipulation::Channel::RED, true);
        break;
    case ImageCanvas::Operation::FLIP_H:
        imgCanvas->requestFlip(false);
        break;
    case ImageCanvas::Operation::FLIP_V:
        imgCanvas->requestFlip(true);
        break;
    case ImageCanvas::Operation::GAUSSIAN_BLUR:
    {
        float gaussian = 0;
        PersistentStorage::getFloat("sidebar", "gaussianValue", &gaussian);
        gaussianSlider->setValue(gaussian);
        break;
    }
    case ImageCanvas::Operation::BRIGHTNESS:
    {
        float brightness = 0;
        PersistentStorage::getFloat("sidebar", "brightnessValue", &brightness);
        brighnessSlider->setValue(brightness);
        break;
    }
    case ImageCanvas::Operation::CONTRAST:
    {
        float contrast = 1;
        PersistentStorage::getFloat("sidebar", "contrastValue", &contrast);
        contrastSlider->setValue(contrast);
        break;
    }
    }


    PersistentStorage::getInt("sidebar", "histRVisible", &tmp);
    redCheckbox->setValue(tmp, false);
    PersistentStorage::getInt("sidebar", "histGVisible", &tmp);
    greenCheckbox->setValue(tmp, false);
    PersistentStorage::getInt("sidebar", "histBVisible", &tmp);
    blueCheckbox->setValue(tmp, false);
    PersistentStorage::getInt("sidebar", "histLumVisible", &tmp);
    luminanceCheckbox->setValue(tmp, false);
}

void SideBar::loadBmp(int n){
    if(loadedImages[n]){
        return;
    }

    PersistentStorage::setInt("sidebar","loadedbmp"+std::to_string(n), 1);


    // carrega o bitmap
    std::string path = "./Trab1RodrigoAppelt/images/" + std::string(1,'a'+n) + ".bmp";
    Bmp *bmp = new Bmp(path.c_str());
    bmp->convertBGRtoRGB();

    // converte para classe Image
    Image *img = new Image(bmp->getWidth(), bmp->getHeight());
    memmove(img->pixels, bmp->getImage(), bmp->getWidth()*bmp->getHeight()*3);
    delete bmp;



    this->imgCanvas->submitImage(img, n);
}

SideBar::~SideBar(){
    delete this->uiManager;
}


void SideBar::draw(){
    CV::translate(pos);
    CV::color(0.255f, 0.353f, 0.467f);
    CV::rectFill(Vector2::zero(), this->size);


    int margin = 5;
    Vector2 tripButtonSize = Vector2((this->size.x-(margin*4))/3, 50);
    Vector2 quadButtonSize = Vector2((this->size.x-(margin*5))/4, 50);
    CV::color(0,0,0);
    CV::text(
        margin,
        tripButtonSize.y+margin*5+quadButtonSize.y*2+150+margin*11,
        "Gaussian Blur",
        GLUT_BITMAP_HELVETICA_10
    );
    CV::text(
        margin,
        tripButtonSize.y+margin*5+quadButtonSize.y*2+150+margin*19,
        "Brightness",
        GLUT_BITMAP_HELVETICA_10
    );
    CV::text(
        margin,
        tripButtonSize.y+margin*5+quadButtonSize.y*2+150+margin*27,
        "Contrast",
        GLUT_BITMAP_HELVETICA_10
    );

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
