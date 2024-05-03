#ifndef __IMAGE_RENDERER_H__
#define __IMAGE_RENDERER_H__

#include <vector>

#include "../Image/ImageManipulation.h"
#include "../Math/Vector2.h"
#include "../Math/Matrix.h"

/// @brief Classe responsavel por renderizar uma
/// imagem na tela
class ImageRenderer {
public:

    ImageRenderer(Vector2 pos, Image *img);
    ~ImageRenderer();
    void draw();
    Vector2 pos,size, realsize;
    Image *img;
    int imageIndex = -1;

    Vector2 scaling = Vector2::one();
    float rotation = 0;
private:
};

#endif
