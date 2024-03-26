#include "ImageCanvas.h"
#include "../gl_canvas2d.h"
#include "../Image/ImageManipulation.h"

bool inside(Vector2 buttonPos, Vector2 buttonSize, Vector2 mousePos);

ImageCanvas::ImageCanvas() : 
    mousePos(Vector2(0,0)), 
    dragging(false),
    selectedImageRenderer(nullptr),
    editionImageRenderer(nullptr){

}

ImageCanvas::~ImageCanvas(){
    std::cout << "Deleting ImageCanvas" << std::endl;
    for(auto img : imgrenderers){
        delete img->img; // deleta a imagem em si
        delete img; // deleta a entidade imagem c posicao e tamanho
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
}

void ImageCanvas::updateMousePos(Vector2 mousePos){
    this->mousePos = mousePos;
}

void ImageCanvas::mouseDown(){
    // selecionar imagem clicada
    for(auto imageRenderer : this->imgrenderers){
        if(inside(imageRenderer->pos, imageRenderer->size, this->mousePos)){
            // clicou na imagem
            // entao a gente tira ela da lista
            // e add na frente
            selectImage(imageRenderer);
            break;
        }
    }

    // comecar a dar drag na imagem

}

void ImageCanvas::mouseUp(){
    // se esta arrastando, para de arrastar
}

void ImageCanvas::update(){
    // if dragging
    //   move selected image to mousePos
}

void ImageCanvas::drawFrame(){
    if(selectedImageRenderer != nullptr){
        int margin = 5;
        
        CV::translate(selectedImageRenderer->pos);
        CV::color(150/255.0f, 102/255.0f, 12/255.0f);
        // top
        CV::rectFill(Vector2(-margin,-margin), Vector2(selectedImageRenderer->size.x+margin,0));
        // left
        CV::rectFill(Vector2(-margin,0), Vector2(0,selectedImageRenderer->size.y+margin));
        // right
        CV::rectFill(Vector2(selectedImageRenderer->size.x,0), Vector2(selectedImageRenderer->size.x+margin, selectedImageRenderer->size.y+margin));
        // bottom
        CV::rectFill(Vector2(-margin,selectedImageRenderer->size.y), Vector2(selectedImageRenderer->size.x+margin, selectedImageRenderer->size.y+margin));
    }
}

void ImageCanvas::submitImage(Image* image){

    int n = imgrenderers.size();
    ImageRenderer *imgrnd = new ImageRenderer(Vector2(5+n*20,5+n*20), image);
    if(selectedImageRenderer == nullptr){
        selectedImageRenderer = imgrnd;
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

}

void ImageCanvas::requestChannel(ImageManipulation::Channel channel, bool luminance){
    if(selectedImageRenderer == nullptr){
        std::cout << "Nao tinha img selecionada" << std::endl;
        return;
    }

    std::cout << "Extraindo canal " << (int)channel << " da imagem(Lumin " << luminance << ")" << std::endl;

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
        std::cout << "Nao tinha img selecionada" << std::endl;
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