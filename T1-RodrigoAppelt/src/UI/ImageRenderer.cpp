#include "ImageRenderer.h"
#include "../gl_canvas2d.h"


void ImageRenderer::draw(Vector2 pos, Image *img){
    CV::translate(pos);
    int w,h;
    img->getSize(&w, &h);
    for(int y = 0; y < h; y++){
        for(int x = 0; x < w; x++){
            int pxStart = (y * w + x) * (int)ImageManipulation::Channel::COUNT;
            uint8_t r = img->pixels[pxStart + (int)ImageManipulation::Channel::RED];
            uint8_t g = img->pixels[pxStart + (int)ImageManipulation::Channel::GREEN];
            uint8_t b = img->pixels[pxStart + (int)ImageManipulation::Channel::BLUE];
            CV::color(r/255.0f, g/255.0f, b/255.0f);
            CV::rectFill(Vector2(x,y), Vector2(1,1));
        }
    }
}
