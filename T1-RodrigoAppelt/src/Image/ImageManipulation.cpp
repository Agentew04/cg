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

void ImageManipulation::Histogram(Image *source, uint32_t* histogram, Channel channel, bool luminance){
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
            int value = source->pixels[i * (int)Channel::COUNT + (int)channel];
            histogram[value]++;
        }
    }
}

void ImageManipulation::Brightness(Image *source, Image *destination, float value){
    int w,h;
    source->getSize(&w, &h);
    int n = w * h;
    for(int i = 0; i < n; i++){
        for(int c = 0; c < (int)Channel::COUNT; c++){
            int pixelStart = i * (int)Channel::COUNT;
            int newValue = source->pixels[pixelStart + c] * value;
            if(newValue > UINT8_MAX){
                newValue = UINT8_MAX;
            }
            destination->pixels[pixelStart + c] = newValue;
        }
    }
}

void ImageManipulation::GaussianBlur(Image *source, Image *destination, float radius){
    float sigma = std::max(radius/2.0f, 1.0f);
    int kernelWidth = 2 * ((int)radius) + 1; // obriga a ser impar p/ ter centro

    float **kernel = new float*[kernelWidth];
    for(int i = 0; i < kernelWidth; i++){
        kernel[i] = new float[kernelWidth];
    }
    float sum = 0.0f;

    // popular o kernel
    for(int x = -(int)radius; x <= radius; x++){
        for(int y = -(int)radius; y <= radius; y++){
            float value = exp(-(x*x + y*y)/(2*sigma*sigma));
            float kernelValue = value / (2 * 3.1415 * sigma * sigma);
            kernel[x + (int)radius][y + (int)radius] = kernelValue;
            sum += kernelValue;
        }
    }

    // agora normaliza o kernel para somar 1
    for(int x = 0; x < kernelWidth; x++){
        for(int y = 0; y < kernelWidth; y++){
            kernel[x][y] /= sum;
        }
    }

    int w,h;
    source->getSize(&w, &h);
    int n = w * h;
    for(int i = 0; i < n; i++){
        int x = i % w;
        int y = i / w;
        for(int c = 0; c < (int)Channel::COUNT; c++){
            float newValue = 0.0f;
            for(int kx = 0; kx < kernelWidth; kx++){
                for(int ky = 0; ky < kernelWidth; ky++){
                    int px = x + kx - (int)radius;
                    int py = y + ky - (int)radius;
                    if(px < 0 || px >= w || py < 0 || py >= h){
                        continue;
                    }
                    int pixelStart = (py * w + px) * (int)Channel::COUNT;
                    newValue += source->pixels[pixelStart + c] * kernel[kx][ky];
                }
            }
            destination->pixels[i * (int)Channel::COUNT + c] = newValue;
        }
    }
}