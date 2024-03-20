#include <iostream>
#include <cstring>

#include "ImageManipulation.h"

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

inline uint8_t toLuminance(uint8_t r, uint8_t g, uint8_t b){
    return 0.299 * r + 0.587 * g + 0.114 * b;
}

void ImageManipulation::ExtractChannel(Image *source, Image *destination, Channel channel){
    int n = source->width * source->height;
    memset(destination->pixels, 0, n * (int)Channel::COUNT);
    for(int i = 0; i < n; i++){
        destination->pixels[i * (int)Channel::COUNT + (int)channel] = source->pixels[i * (int)Channel::COUNT + (int)channel];
    }
}

void ImageManipulation::CopyImage(Image *source, Image *destination){
    int n = source->width * source->height * (int)Channel::COUNT;
    memcpy(destination->pixels, source->pixels, n);
}

void ImageManipulation::ToGrayscale(Image *source, Image *destination){
    int n = source->width * source->height;
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
    for(int y = 0; y < source->height; y++){ // itera pelas linhas
        for(int x = 0; x < source->width; x++){ // p/ cada linha, inverte
            int pxStart = (y * source->width + x) * (int)Channel::COUNT;
            int inversePxStart = (y * source->width + (source->width - x - 1)) * (int)Channel::COUNT;

            for(int c = 0; c < (int)Channel::COUNT; c++){
                destination->pixels[inversePxStart + c] = source->pixels[pxStart + c];
            }
        }
    }
}

void ImageManipulation::FlipVertical(Image *source, Image *destination){
    for(int y = 0; y < source->height; y++){
        for(int x = 0; x < source->width; x++){
            int pxStart = (y * source->width + x) * (int)Channel::COUNT;
            int inversePxStart = ((source->height - y - 1) * source->width + x) * (int)Channel::COUNT;

            for(int c = 0; c < (int)Channel::COUNT; c++){
                destination->pixels[inversePxStart + c] = source->pixels[pxStart + c];
            }
        }
    }
}

void ImageManipulation::Histogram(Image *source, uint8_t* histogram, Channel channel, bool luminance){
    int n = source->width * source->height;
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
