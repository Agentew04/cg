#include "ImageRenderer.h"
#include "../gl_canvas2d.h"

#include <iostream>

ImageRenderer::ImageRenderer(Vector2 pos, Vector2 size, Image *img)
    : pos(pos), size(size), img(img){
}

ImageRenderer::~ImageRenderer(){
    std::cout << "Deleting ImageRenderer" << std::endl;
}

void ImageRenderer::draw(){
    CV::translate(pos);
    int w,h;
    img->getSize(&w, &h);
    for(int y = 0; y < h; y++){
        int yinv = h - y - 1;
        for(int x = 0; x < w; x++){
            int pxStart = (yinv * w + x) * (int)ImageManipulation::Channel::COUNT;
            uint8_t r = img->pixels[pxStart + (int)ImageManipulation::Channel::RED];
            uint8_t g = img->pixels[pxStart + (int)ImageManipulation::Channel::GREEN];
            uint8_t b = img->pixels[pxStart + (int)ImageManipulation::Channel::BLUE];
            CV::color((float)r/255.0f, (float)g/255.0f, (float)b/255.0f);
            CV::rectFill(Vector2(x,y), Vector2(x+1,y+1));
        }
    }
}
