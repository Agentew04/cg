#include "Buffer.h"

#include <iostream>
#include <GL/glut.h>

Buffer::Buffer(int width, int height, int channels) : 
    width(width),
    height(height),
    channels(channels){
    buffer = new float[width * height * channels];
    if(buffer == nullptr){
        std::cerr << "Erro ao alocar buffer!" << std::endl;
        exit(1);
    }
}

Buffer::~Buffer(){
    delete[] buffer;
}

void Buffer::display(float upscale) const {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    std::cout << "diplaying buffer with upscale: " << upscale << std::endl;

    if(channels == 3){
        displayColor(upscale);
    }else if(channels == 1){
        displayGray(upscale);
    }else{
        std::cout << "Nao sei mostrar buffer com " << channels << " canais!" << std::endl;
    }
    // mostrar uma borda
    glColor3f(0, 0, 0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(0, 0);
    glVertex2f(width*upscale, 0);
    glVertex2f(width*upscale, height*upscale);
    glVertex2f(0, height*upscale);
    glEnd();
}

void Buffer::displayColor(float upscale) const {
    float pixelSize = 1.0 * upscale;
    glBegin(GL_QUADS);
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            float r = buffer[getOffset(x, y)];
            float g = buffer[getOffset(x, y) + 1];
            float b = buffer[getOffset(x, y) + 2];

            glColor3f(r, g, b);
            glVertex2f(x*pixelSize, y*pixelSize);
            glVertex2f(x*pixelSize + pixelSize, y*pixelSize);
            glVertex2f(x*pixelSize + pixelSize, y*pixelSize + pixelSize);
            glVertex2f(x*pixelSize, y*pixelSize + pixelSize);
        }
    }
    glEnd();
}

void Buffer::displayGray(float upscale) const {
    glBegin(GL_POINTS);
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            int offset = getOffset(x, y);
            glColor3f(buffer[offset], buffer[offset], buffer[offset]);
            glVertex2f(x, y);
        }
    }
    glEnd();
}

void Buffer::fill(float red, float green, float blue){
    if(channels != 3){
        std::cerr << "Nao sei preencher buffer com " << channels << " canais!" << std::endl;
        return;
    }
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            int offset = getOffset(x, y);
            buffer[offset] = red;
            buffer[offset + 1] = green;
            buffer[offset + 2] = blue;
        }
    }
}

void Buffer::fill(float gray){
    if(channels != 1){
        std::cerr << "Nao sei preencher buffer com " << channels << " canais!" << std::endl;
        return;
    }
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            int offset = getOffset(x, y);
            buffer[offset] = gray;
        }
    }
}

void Buffer::resize(int width, int height){
    delete[] buffer;
    this->width = width;
    this->height = height;
    buffer = new float[width * height * channels];
    if(buffer == nullptr){
        std::cerr << "Erro ao alocar buffer!" << std::endl;
        exit(1);
    }
}

void Buffer::resizeIfNeeded(int width, int height){
    if(this->width != width || this->height != height){
        resize(width, height);
    }
}

void Buffer::getSize(float &width, float &height) const {
    width = this->width;
    height = this->height;
}
