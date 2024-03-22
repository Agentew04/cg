#ifndef __IMAGE_RENDERER_H__
#define __IMAGE_RENDERER_H__

#include "../Image/ImageManipulation.h"
#include "../Vector2.h"

/// @brief Classe responsavel por renderizar uma
/// imagem na tela
class ImageRenderer {
public:

    ImageRenderer(Vector2 pos, Image *img);
    ~ImageRenderer();
    void draw();
    Vector2 pos,size;
    Image *img;
private:
};

#endif