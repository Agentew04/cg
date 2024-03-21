#ifndef __IMAGE_RENDERER_H__
#define __IMAGE_RENDERER_H__

#include "../Image/ImageManipulation.h"
#include "../Vector2.h"

/// @brief Classe responsavel por renderizar uma
/// imagem na tela
class ImageRenderer {
public:

    /// @brief Desenha a imagem na tela
    /// @param pos A posicao onde a imagem sera desenhada
    /// @param img A imagem a ser desenhada
    static void draw(Vector2 pos, Image *img);
};

#endif