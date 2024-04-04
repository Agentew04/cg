#include <string.h>

#include "ImageCanvas.h"
#include "../gl_canvas2d.h"
#include "../Image/ImageManipulation.h"
#include "../UI/CursorManager.h"
#include "../Storage/PersistentStorage.h"

bool inside(Vector2 buttonPos, Vector2 buttonSize, Vector2 mousePos);

ImageCanvas::ImageCanvas() :
    selectedImageRenderer(nullptr),
    editionImageRenderer(nullptr),
    mousePos(Vector2(0,0)),
    dragging(false),
    histR(nullptr), histG(nullptr), histB(nullptr), histLum(nullptr){

}

ImageCanvas::~ImageCanvas(){
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
    if(holdingScaleH || holdingScaleV){
        checkScaleMovement();
    }
}

void ImageCanvas::mouseDown(){
    // selecionar imagem clicada
    for(auto imageRenderer : this->imgrenderers){
        auto scale = imageRenderer->scaling;
        std::vector<Vector2> points = {
            Vector2::zero(),
            Vector2(imageRenderer->realsize.x*scale.x, 0),
            imageRenderer->realsize*scale,
            Vector2(0, imageRenderer->realsize.y*scale.y)
        };
        Polygon2D imgCollider(points);
        if (imgCollider.pointInside(mousePos - imageRenderer->pos)) {
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
    if(selectedImageRenderer != nullptr){
        if(frameRight.pointInside(mousePos - selectedImageRenderer->pos)){
            holdingScaleH = true;
        }
        if(frameBottom.pointInside(mousePos - selectedImageRenderer->pos)){
            holdingScaleV = true;
        }
    }
}

void ImageCanvas::mouseUp(){
    // se esta arrastando, para de arrastar
    this->dragging = false;
    holdingScaleH = false;
    holdingScaleV = false;
}

void ImageCanvas::update(){
    if(this->dragging && this->selectedImageRenderer != nullptr){
        this->selectedImageRenderer->pos = this->mousePos - dragPivot;
        std::cout << "update::selectedImageRenderer::imageIndex = " << selectedImageRenderer->imageIndex << std::endl;
        PersistentStorage::setVec2("imgState", "pos"+std::to_string(selectedImageRenderer->imageIndex), selectedImageRenderer->pos);
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
    CV::polygonFill(Vector2::zero(), &frameTop);
    // right
    CV::polygonFill(Vector2::zero(), &frameRight);
    // bottom
    CV::polygonFill(Vector2::zero(), &frameBottom);
    // left
    CV::polygonFill(Vector2::zero(), &frameLeft);

    // rotation knob <== desligado, nao consegui implementar rotacao
    //CV::color(rotationKnobColor);
    // CV::circleFill(
    //     (selectedImageRenderer->realsize*selectedImageRenderer->scaling)
    //     + Vector2(margin/2.0f, margin/2.0f),
    //     margin*2, 10);
}

void ImageCanvas::submitImage(Image* image, int imageId){
    int n = imgrenderers.size();
    std::cout << "id: " << imageId << std::endl;

    PersistentStorage::setIfNotVec2("imgState", "pos"+std::to_string(imageId), Vector2(5+n*20,5+n*20));

    Vector2 imgpos;
    PersistentStorage::getVec2("imgState", "pos"+std::to_string(imageId), &imgpos);
    ImageRenderer *imgrnd = new ImageRenderer(imgpos, image);
    imgrnd->imageIndex = imageId;
    if(selectedImageRenderer == nullptr){
        selectImage(imgrnd);
    }else{
        imgrenderers.emplace_back(imgrnd);
    }
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
    updateBrightness(1.0);

    // cria os poligonos do frame
    calculateFrames();
}

void ImageCanvas::requestChannel(ImageManipulation::Channel channel, bool luminance){
    if(selectedImageRenderer == nullptr){
        return;
    }

    Image *source = selectedImageRenderer->img;
    Image *target = editionImageRenderer->img;
    if(!luminance){
        ImageManipulation::ExtractChannel(source, target, channel);
    }else{
        ImageManipulation::ToGrayscale(source, target);
    }
}

void ImageCanvas::calculateFrames(){
    if(selectedImageRenderer == nullptr){
        return;
    }

    auto imgrnd = selectedImageRenderer;

    int margin = 5;
    auto scale = imgrnd->scaling;
    frameLeft = Polygon2D({
        Vector2(-margin,-margin),
        Vector2(0, -margin),
        Vector2(0, imgrnd->realsize.y*scale.y),
        Vector2(-margin, imgrnd->realsize.y*scale.y)
    });
    frameRight = Polygon2D({
        Vector2(imgrnd->realsize.x*scale.x, 0),
        Vector2(imgrnd->realsize.x*scale.x+margin, 0),
        Vector2(imgrnd->realsize.x*scale.x+margin, imgrnd->realsize.y*scale.y+margin),
        Vector2(imgrnd->realsize.x*scale.x, imgrnd->realsize.y*scale.y+margin)
    });
    frameTop = Polygon2D({
        Vector2(0, -margin),
        Vector2(imgrnd->realsize.x*scale.x + margin, -margin),
        Vector2(imgrnd->realsize.x*scale.x + margin, 0),
        Vector2(0, 0)
    });
    frameBottom = Polygon2D({
        Vector2(-margin,imgrnd->realsize.y*scale.y),
        Vector2(imgrnd->realsize.x*scale.x, imgrnd->realsize.y*scale.y),
        Vector2(imgrnd->realsize.x*scale.x, imgrnd->realsize.y*scale.y + margin),
        Vector2(-margin, imgrnd->realsize.y*scale.y + margin)
    });
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
        if(inside(imageRenderer->pos, imageRenderer->realsize*imageRenderer->scaling, this->mousePos)){
            CursorManager::setCursor(CursorManager::CursorType::CLICKABLE);
        }
    }

    if(selectedImageRenderer == nullptr){
        return;
    }
    if(inside(selectedImageRenderer->pos, selectedImageRenderer->realsize*selectedImageRenderer->scaling, this->mousePos)){
        CursorManager::setCursor(CursorManager::CursorType::MOVE);
    }

    int margin = 5;
    // na borda da esq ou direita

    if(/*frameLeft.pointInside(mousePos - selectedImageRenderer->pos)
    || */frameRight.pointInside(mousePos - selectedImageRenderer->pos)){
        CursorManager::setCursor(CursorManager::CursorType::RESIZE_HORIZONTAL);
    }

    // na borda de cima ou de baixo
    if(/*frameTop.pointInside(mousePos - selectedImageRenderer->pos)
        || */frameBottom.pointInside(mousePos - selectedImageRenderer->pos)){
        CursorManager::setCursor(CursorManager::CursorType::RESIZE_VERTICAL);
    }

    // no knob de rotacao <== desabilitado, n implementado
    // if((selectedImageRenderer->pos +
    //     (selectedImageRenderer->realsize*selectedImageRenderer->scaling) +
    //     Vector2(margin/2.0f, margin/2.0f)
    // ).distance(mousePos) <= margin*2){
    //     CursorManager::setCursor(CursorManager::CursorType::ROTATE);
    // }
}

inline int getMax(uint32_t *begin, uint32_t *end){
    uint32_t max = 0;
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

void ImageCanvas::updateBrightness(float value){
    if(selectedImageRenderer == nullptr){
        return;
    }
    ImageManipulation::Brightness(selectedImageRenderer->img, editionImageRenderer->img, value);
}

void ImageCanvas::updateContrast(float value){
    if(selectedImageRenderer == nullptr){
        return;
    }
    ImageManipulation::Contrast(selectedImageRenderer->img, editionImageRenderer->img, value);
}

void ImageCanvas::checkScaleMovement(){
    if(selectedImageRenderer == nullptr){
        return;
    }

    // Vector2 relativePos = Vector2(mousePos.x - selectedImageRenderer->pos.x, mousePos.y - selectedImageRenderer->pos.y);
    if(holdingScaleH){
        // redimensionar horizontal p/ direito
        float scalePerPixel = 1.0/selectedImageRenderer->realsize.x;
        int newPixels = mousePos.x - selectedImageRenderer->pos.x - selectedImageRenderer->realsize.x;
        float newScale = scalePerPixel * newPixels + 1.0;
        selectedImageRenderer->scaling.x = newScale;
        selectedImageRenderer->size.x = newPixels + selectedImageRenderer->size.x;
    }
    if(holdingScaleV){
        // redimensionar vertical
        float scalePerPixel = 1.0/selectedImageRenderer->realsize.y;
        int newPixels = mousePos.y - selectedImageRenderer->pos.y - selectedImageRenderer->realsize.y;
        float newScale = scalePerPixel * newPixels + 1.0;
        selectedImageRenderer->scaling.y = newScale;
        selectedImageRenderer->size.y = newPixels + selectedImageRenderer->size.y;
    }
    PersistentStorage::setVec2(
        "imgState", 
        "scale"+std::to_string(selectedImageRenderer->imageIndex), 
        selectedImageRenderer->scaling    
    );
    if(holdingScaleH || holdingScaleV){
        calculateFrames();
    }
}

void ImageCanvas::updateGaussian(float value){
    if(selectedImageRenderer == nullptr){
        return;
    }
    ImageManipulation::GaussianBlur(selectedImageRenderer->img, editionImageRenderer->img, value);
}
