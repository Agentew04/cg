#include <iterator>

#include "SideBar.h"
#include "../gl_canvas2d.h"
#include "../Image/Bmp.h"

SideBar::SideBar(Vector2 pos, Vector2 size, int *scrW, int *scrH)
    : pos(pos), size(size){
    this->uiManager = new UIManager();

    selectedImage = nullptr;
    submitButtons();
    //submitHistogram();
    //submitImagePreview();
}

void SideBar::submitButtons(){
    ButtonStyle *btnstl = ButtonStyle::Windows10();
    Button *ldbtn1 = new Button(pos + Vector2(5,5), Vector2(100,50), "Load 1", [&](Button* but){
        loadBmp(0);
        but->clickable = false;
    });
    ldbtn1->style = btnstl;
    this->uiManager->add(ldbtn1);

    Button *ldbtn2 = new Button(pos + Vector2(110,5), Vector2(100,50), "Load 2", [&](Button* but){
        loadBmp(1);
        but->clickable = false;
    });
    ldbtn2->style = btnstl;
    this->uiManager->add(ldbtn2);

    Button *ldbtn3 = new Button(pos + Vector2(215,5), Vector2(100,50), "Load 3", [&](Button* but){
        loadBmp(2);
        but->clickable = false;
    });
    ldbtn3->style = btnstl;
    this->uiManager->add(ldbtn3);
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
    int w,h;
    img->getSize(&w, &h);

    // atualiza as referencias e tals
    // imgrnd nao vai ser o dono do ponteiro img, a gente desaloca nessa classe mesmo
    ImageRenderer *imgrnd = new ImageRenderer(Vector2(5+n*20,5+n*20), img);

    if(selectedImage == nullptr){
        selectedImage = img;
    }
    imgrenderers.push_back(imgrnd);
    loadedImages[n] = true;
    imgToRenderer[img] = imgrnd;
    images.push_back(img);
    this->uiManager->add(imgrnd);
    sortImages();
}

SideBar::~SideBar(){
    delete this->uiManager;
    for(auto img : images){
        delete img;
    }
    for(auto imgrnd : imgrenderers){
        delete imgrnd;
    }
    loadedImages.clear();
    imgrenderers.clear();
}

void SideBar::draw(){
    CV::translate(pos);
    CV::color(0,1,0);
    CV::rectFill(Vector2::zero(), this->size);

    this->uiManager->draw();

    // desenha a moldura
    if(selectedImage != nullptr){
        int margin = 5;
        ImageRenderer* selectedRenderer = imgToRenderer[selectedImage];
        CV::translate(selectedRenderer->pos);
        CV::color(150/255.0f, 102/255.0f, 12/255.0f);
        // top
        CV::rectFill(Vector2(-margin,-margin), Vector2(selectedRenderer->size.x+margin,0));
        // left
        CV::rectFill(Vector2(-margin,0), Vector2(0,selectedRenderer->size.y+margin));
        // right
        CV::rectFill(Vector2(selectedRenderer->size.x,0), Vector2(selectedRenderer->size.x+margin, selectedRenderer->size.y+margin));
        // bottom
        CV::rectFill(Vector2(-margin,selectedRenderer->size.y), Vector2(selectedRenderer->size.x+margin, selectedRenderer->size.y+margin));
    }
}

void SideBar::updateMousePos(Vector2 mousePos){
    this->mousePos = mousePos;
    this->uiManager->updateMousePos(mousePos);
}

bool inside(Vector2 buttonPos, Vector2 buttonSize, Vector2 mousePos);

void SideBar::sortImages(){
    for(auto ir : this->imgrenderers){
        this->uiManager->remove(ir);
    }
    for(int i=this->imgrenderers.size()-1; i>=0; i--){
        this->uiManager->add(this->imgrenderers[i]);
    }
}

void SideBar::mouseDown(){
    this->uiManager->mouseDown();

    // procurar se o click foi em uma das imagens
    // ja esta em ordem de prioridade
    for(auto imgrnd : this->imgrenderers){
        if(inside(imgrnd->pos, imgrnd->size, mousePos)){
            std::cout << "Clicked on image " << imgrnd->img << std::endl;
            
            this->selectedImage = imgrnd->img;

            // tira a imagem da lista
            for(auto it = this->imgrenderers.begin(); it != this->imgrenderers.end(); it++){
                if(*it == imgrnd){
                    this->imgrenderers.erase(it);
                    break;
                }
            }
            this->imgrenderers.emplace(this->imgrenderers.begin(), imgrnd);

            sortImages();
            break;
        }
    }
}

void SideBar::mouseUp(){
    this->uiManager->mouseUp();
}
