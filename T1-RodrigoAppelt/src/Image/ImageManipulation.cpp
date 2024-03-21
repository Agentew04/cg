#include <iostream>
#include <cstring>

#include "ImageManipulation.h"
#include "../Vector2.h"

Image::Image(int width, int height){
    this->width = width;
    this->height = height;
    this->pixels = new uint8_t[width * height * (int)ImageManipulation::Channel::COUNT];
    std::cout << "aloquei uma imagem de " << width << "x" << height << std::endl;
}

Image::~Image(){
    delete[] this->pixels;
    std::cout << "desalocando uma imagem de " << this->width << "x" << this->height << std::endl;
}

Vector2 Image::getSize(){
    return Vector2(this->width, this->height);
}

void Image::getSize(int *w, int *h){
    *w = this->width;
    *h = this->height;
}

inline uint8_t toLuminance(uint8_t r, uint8_t g, uint8_t b){
    return 0.299 * r + 0.587 * g + 0.114 * b;
}

void ImageManipulation::ExtractChannel(Image *source, Image *destination, Channel channel){
    int w,h;
    source->getSize(&w, &h);
    int n = w * h;
    memset(destination->pixels, 0, n * (int)Channel::COUNT);
    for(int i = 0; i < n; i++){
        destination->pixels[i * (int)Channel::COUNT + (int)channel] = source->pixels[i * (int)Channel::COUNT + (int)channel];
    }
}

void ImageManipulation::CopyImage(Image *source, Image *destination){
    int w,h;
    source->getSize(&w, &h);
    int n = w * h * (int)Channel::COUNT;
    memcpy(destination->pixels, source->pixels, n);
}

void ImageManipulation::ToGrayscale(Image *source, Image *destination){
    int w,h;
    source->getSize(&w, &h);
    int n = w * h;
    for(int i = 0; i < n; i++){
        int pixelStart = i * (int)Channel::COUNT;
        uint8_t r = source->pixels[pixelStart + (int)Channel::RED];
        uint8_t g = source->pixels[pixelStart + (int)Channel::GREEN];
        uint8_t b = source->pixels[pixelStart + (int)Channel::BLUE];
        uint8_t gray = toLuminance(r,g,b);
        destination->pixels[i * (int)Channel::COUNT + (int)Channel::RED] = gray;
        destination->pixels[i * (int)Channel::COUNT + (int)Channel::GREEN] = gray;
        destination->pixels[i * (int)Channel::COUNT + (int)Channel::BLUE] = gray;
    }
}

void ImageManipulation::FlipHorizontal(Image *source, Image *destination){
    int w,h;
    source->getSize(&w, &h);
    for(int y = 0; y < h; y++){ // itera pelas linhas
        for(int x = 0; x < w; x++){ // p/ cada linha, inverte
            int pxStart = (y * w + x) * (int)Channel::COUNT;
            int inversePxStart = (y * w + (w - x - 1)) * (int)Channel::COUNT;

            for(int c = 0; c < (int)Channel::COUNT; c++){
                destination->pixels[inversePxStart + c] = source->pixels[pxStart + c];
            }
        }
    }
}

void ImageManipulation::FlipVertical(Image *source, Image *destination){
    int w,h;
    source->getSize(&w, &h);
    for(int y = 0; y < h; y++){
        for(int x = 0; x < w; x++){
            int pxStart = (y * w + x) * (int)Channel::COUNT;
            int inversePxStart = ((h - y - 1) * w + x) * (int)Channel::COUNT;

            for(int c = 0; c < (int)Channel::COUNT; c++){
                destination->pixels[inversePxStart + c] = source->pixels[pxStart + c];
            }
        }
    }
}

void ImageManipulation::Histogram(Image *source, uint8_t* histogram, Channel channel, bool luminance){
    int w,h;
    source->getSize(&w, &h);
    int n = w * h;
    memset(histogram, 0, 256);
    if(luminance){
        for(int i = 0; i < n; i++){
            int pixelStart = i * (int)Channel::COUNT;
            uint8_t r = source->pixels[pixelStart + (int)Channel::RED];
            uint8_t g = source->pixels[pixelStart + (int)Channel::GREEN];
            uint8_t b = source->pixels[pixelStart + (int)Channel::BLUE];
            histogram[toLuminance(r,g,b)]++;
        }
    }else{
        for(int i = 0; i < n; i++){
            histogram[source->pixels[i * (int)Channel::COUNT + (int)channel]]++;
        }
    }
}
