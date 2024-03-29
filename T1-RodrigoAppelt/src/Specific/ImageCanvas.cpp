#include <string.h>

#include "ImageCanvas.h"
#include "../gl_canvas2d.h"
#include "../Image/ImageManipulation.h"
#include "../UI/CursorManager.h"

bool inside(Vector2 buttonPos, Vector2 buttonSize, Vector2 mousePos);

ImageCanvas::ImageCanvas() : 
    mousePos(Vector2(0,0)), 
    dragging(false),
    selectedImageRenderer(nullptr),
    editionImageRenderer(nullptr),
    histR(nullptr), histG(nullptr), histB(nullptr), histLum(nullptr){

}

ImageCanvas::~ImageCanvas(){
    std::cout << "Deleting ImageCanvas" << std::endl;
    for(auto img : imgrenderers){
        delete img->img; // deleta a imagem em si
        delete img; // deleta a entidade imagem c posicao e tamanho
    }
    if(editionImageRenderer != nullptr){
        delete editionImageRenderer->img;
        delete editionImageRenderer;
    }
}

void ImageCanvas::draw(){
    for(auto riter = imgrenderers.rbegin(); riter != imgrenderers.rend(); ++riter){
        (*riter)->draw();
    }

    drawFrame();

    if(editionImageRenderer != nullptr){
        editionImageRenderer->draw();
    }

    updateCursor();
}

void ImageCanvas::updateMousePos(Vector2 mousePos){
    this->mousePos = mousePos;
    updateCursor();
}

void ImageCanvas::mouseDown(){
    // selecionar imagem clicada
    for(auto imageRenderer : this->imgrenderers){
        if(inside(imageRenderer->pos, imageRenderer->size, this->mousePos)){
            // clicou na imagem
            // entao a gente tira ela da lista
            // e add na frente
            selectImage(imageRenderer);
            dragging = true;
            dragPivot = this->mousePos - selectedImageRenderer->pos;
            break;
        }
    }

    // comecar a dar drag na imagem

}

void ImageCanvas::mouseUp(){
    // se esta arrastando, para de arrastar
    this->dragging = false;
}

void ImageCanvas::update(){
    if(this->dragging && this->selectedImageRenderer != nullptr){
        // TODO contabilizar ponto de pivo
        this->selectedImageRenderer->pos = this->mousePos - dragPivot;
    }
}

void ImageCanvas::drawFrame(){
    if(selectedImageRenderer == nullptr){
        return;
    }
    int margin = 5;

    int sizeX = selectedImageRenderer->size.x;
    int sizeY = selectedImageRenderer->size.y;
    Vector3 frameColor = Vector3(0.051f, 0.106f, 0.165f);
    Vector3 rotationKnobColor = Vector3(0.878f, 0.882f, 0.867f);
    CV::translate(selectedImageRenderer->pos);
    CV::color(frameColor);
    // top
    CV::rectFill(Vector2(0,-margin), Vector2(sizeX+margin,0));
    // right
    CV::rectFill(Vector2(sizeX,0), Vector2(sizeX+margin, sizeY));
    // bottom
    CV::rectFill(Vector2(-margin, sizeY), Vector2(sizeX, sizeY+margin));
    // left
    CV::rectFill(Vector2(-margin, -margin), Vector2(0, sizeY));

    // rotation knob
    CV::color(rotationKnobColor);
    CV::circleFill(sizeX + margin/2.0f, sizeY + margin/2.0f, margin*2, 10);
}

void ImageCanvas::submitImage(Image* image){

    int n = imgrenderers.size();
    ImageRenderer *imgrnd = new ImageRenderer(Vector2(5+n*20,5+n*20), image);
    if(selectedImageRenderer == nullptr){
        selectedImageRenderer = imgrnd;
        int w,h;
        image->getSize(&w, &h);
        editionImageRenderer = new ImageRenderer(Vector2(500,300), new Image(w,h));
        ImageManipulation::CopyImage(imgrnd->img, editionImageRenderer->img);
    }
    imgrenderers.push_back(imgrnd);
    imgToRenderer[image] = imgrnd;
}

void ImageCanvas::selectImage(ImageRenderer* imgrnd){
    // atualiza a imagem de edicao
    if(this->selectedImageRenderer != imgrnd){
        if(editionImageRenderer != nullptr){
            delete editionImageRenderer->img;
            delete editionImageRenderer;
        }
        int w,h;
        imgrnd->img->getSize(&w, &h);
        editionImageRenderer = new ImageRenderer(Vector2(500,300), new Image(w,h));
        ImageManipulation::CopyImage(imgrnd->img, editionImageRenderer->img);
    }

    // prioridade de renderizacao/clique
    this->selectedImageRenderer = imgrnd;
    for(auto it = this->imgrenderers.begin(); it != this->imgrenderers.end(); it++){
        if(*it == imgrnd){
            this->imgrenderers.erase(it);
            break;
        }
    }
    this->imgrenderers.emplace(this->imgrenderers.begin(), imgrnd);

    // calcula os histogramas de cada canal
    updateSelectedHistograms();
}

void ImageCanvas::requestChannel(ImageManipulation::Channel channel, bool luminance){
    if(selectedImageRenderer == nullptr){
        return;
    }

    // aqui n precisa desalocar a img pq o formato n muda
    // e o tamanho vai atualizar quando a img for selecionada

    Image *source = selectedImageRenderer->img;
    Image *target = editionImageRenderer->img;
    if(!luminance){
        ImageManipulation::ExtractChannel(source, target, channel);
    }else{
        ImageManipulation::ToGrayscale(source, target);
    }
}

void ImageCanvas::requestFlip(bool vertical){
    if(selectedImageRenderer == nullptr){
        return;
    }

    int w,h;
    selectedImageRenderer->img->getSize(&w, &h);

    Image *temp = new Image(w,h);
    Image *target = editionImageRenderer->img;
    ImageManipulation::CopyImage(target,temp);

    std::cout << "Flipando imagem " << (vertical ? "vertical" : "horizontal") << std::endl;
    if(!vertical){
        ImageManipulation::FlipHorizontal(temp, target);
    }else{
        ImageManipulation::FlipVertical(temp, target);

    }

}

void ImageCanvas::setHistograms(Chart::Series *histR, Chart::Series *histG, Chart::Series *histB, Chart::Series *histLum){
    this->histR = histR;
    this->histG = histG;
    this->histB = histB;
    this->histLum = histLum;
}

void ImageCanvas::updateCursor(){

    // possibilidade de click se passar mouse nas imagem
    for(auto imageRenderer : this->imgrenderers){
        if(inside(imageRenderer->pos, imageRenderer->size, this->mousePos)){
            CursorManager::setCursor(CursorManager::CursorType::CLICKABLE);
        }
    }

    if(selectedImageRenderer == nullptr){
        return;
    }
    if(inside(selectedImageRenderer->pos, selectedImageRenderer->size, this->mousePos)){
        CursorManager::setCursor(CursorManager::CursorType::MOVE);
    }

    // na borda da esq ou direita
    int margin = 5;
    if(inside(Vector2(selectedImageRenderer->pos.x - margin, selectedImageRenderer->pos.y), Vector2(5, selectedImageRenderer->size.y), this->mousePos)){
        CursorManager::setCursor(CursorManager::CursorType::RESIZE_HORIZONTAL);
    }
    if(inside(Vector2(selectedImageRenderer->pos.x + selectedImageRenderer->size.x, selectedImageRenderer->pos.y), Vector2(margin, selectedImageRenderer->size.y), this->mousePos)){
        CursorManager::setCursor(CursorManager::CursorType::RESIZE_HORIZONTAL);
    }

    // na borda de cima ou de baixo
    if(inside(Vector2(selectedImageRenderer->pos.x, selectedImageRenderer->pos.y - margin), Vector2(selectedImageRenderer->size.x, margin), this->mousePos)){
        CursorManager::setCursor(CursorManager::CursorType::RESIZE_VERTICAL);
    }
    if(inside(Vector2(selectedImageRenderer->pos.x, selectedImageRenderer->pos.y + selectedImageRenderer->size.y), Vector2(selectedImageRenderer->size.x, margin), this->mousePos)){
        CursorManager::setCursor(CursorManager::CursorType::RESIZE_VERTICAL);
    }

    // no knob de rotacao
    if(Vector2(
        selectedImageRenderer->pos.x + selectedImageRenderer->size.x + margin/2.0f, 
        selectedImageRenderer->pos.y + selectedImageRenderer->size.y + margin/2.0f
        ).distance(mousePos) <= margin*2){
        CursorManager::setCursor(CursorManager::CursorType::ROTATE);
    }
}

inline int getMax(uint32_t *begin, uint32_t *end){
    int max = 0;
    for(uint32_t *it = begin; it != end; it++){
        if(*it > max){
            max = *it;
        }
    }
    return max;
}


void ImageCanvas::updateSelectedHistograms(){
    int w,h;
    auto imgrnd = this->selectedImageRenderer;
    imgrnd->img->getSize(&w, &h);
    std::cout << "rate " << maxHistogramValueRatio << std::endl;
    if(histR != nullptr){
        memset(histR->y, 0, sizeof(uint32_t)*(UINT8_MAX+1));
        ImageManipulation::Histogram(imgrnd->img, histR->y, ImageManipulation::Channel::RED, false);
        int max = getMax(histR->y, histR->y + UINT8_MAX+1);
        histR->yBounds.y = (int)(max * maxHistogramValueRatio);
    }
    if(histG != nullptr){
        memset(histG->y, 0, sizeof(uint32_t)*(UINT8_MAX+1));
        ImageManipulation::Histogram(imgrnd->img, histG->y, ImageManipulation::Channel::GREEN, false);
        int max = getMax(histG->y, histG->y + UINT8_MAX+1);
        histG->yBounds.y = (int)(max * maxHistogramValueRatio);
    }
    if(histB != nullptr){
        memset(histB->y, 0, sizeof(uint32_t)*(UINT8_MAX+1));
        ImageManipulation::Histogram(imgrnd->img, histB->y, ImageManipulation::Channel::BLUE, false);
        int max = getMax(histB->y, histB->y + UINT8_MAX+1);
        histB->yBounds.y = (int)(max * maxHistogramValueRatio);
    }
    if(histLum != nullptr){
        memset(histLum->y, 0, sizeof(uint32_t)*(UINT8_MAX+1));
        ImageManipulation::Histogram(imgrnd->img, histLum->y, ImageManipulation::Channel::RED, true);
        int max = getMax(histLum->y, histLum->y + UINT8_MAX+1);
        histLum->yBounds.y = (int)(max * maxHistogramValueRatio);
    }
}

