#include "ImageRenderer.h"
#include "../gl_canvas2d.h"

#include <iostream>

ImageRenderer::ImageRenderer(Vector2 pos, Image *img)
    : pos(pos), img(img){
        this->size = img->getSize();
        this->realsize = size;
}

ImageRenderer::~ImageRenderer(){
    std::cout << "Deleting ImageRenderer" << std::endl;
    // NAO DESALOCAR A IMAGEM AQUI
    // essa classe nao eh dona do ponteiro
}

void ImageRenderer::draw(){
    CV::translate(pos);
    int w,h;
    img->getSize(&w, &h);
    Matrix rotM = Matrix::scale2D(scaling.x,scaling.y);
    for(int y = 0; y < h; y++){
        int yinv = h - y - 1; // pq o y cresce p baixo
        for(int x = 0; x < w; x++){
            int pxStart = (yinv * w + x) * (int)ImageManipulation::Channel::COUNT;
            uint8_t r = img->pixels[pxStart + (int)ImageManipulation::Channel::RED];
            uint8_t g = img->pixels[pxStart + (int)ImageManipulation::Channel::GREEN];
            uint8_t b = img->pixels[pxStart + (int)ImageManipulation::Channel::BLUE];
            CV::color((float)r/255.0f, (float)g/255.0f, (float)b/255.0f);
            Vector2 pos = rotM*(Vector2(x,y));
            CV::rectFill(pos, Vector2(pos.x+scaling.x,pos.y+scaling.y));
        }
    }
}
