#ifndef __IMAGEMANIPULATION_H__
#define __IMAGEMANIPULATION_H__

#include <cstdint>

#include "../Vector2.h"

class Image {
public:
    uint8_t *pixels;
    Image(int width, int height);
    ~Image();
    Vector2 getSize();
    void getSize(int *w, int *h);
private:
    int width, height;
};

class ImageManipulation{
public:
    enum class Channel {
        RED,
        GREEN,
        BLUE,
        COUNT
    };

    /// @brief Extrai um canal de uma imagem para outra.
    /// Os outros canais serao 0. 
    /// @param source 
    /// @param destination 
    /// @param channel 
    static void ExtractChannel(Image *source, Image *destination, Channel channel);

    /// @brief Copia uma imagem para outra
    /// @param source 
    /// @param destination 
    static void CopyImage(Image *source, Image *destination);

    /// @brief Converte uma imagem para escala de cinza em luminancia. Coloca o mesmo
    /// valor nos canais R, G e B de destino
    /// @param source A imagem fonte
    /// @param destination A imagem que recebera o resultado
    static void ToGrayscale(Image *source, Image *destination);

    /// @brief Vira uma imagem no sentido horizontal(espelhar)
    /// @param source 
    /// @param destination 
    static void FlipHorizontal(Image *source, Image *destination);

    /// @brief Vira uma imagem de ponta cabeca
    /// @param source 
    /// @param destination 
    static void FlipVertical(Image *source, Image *destination);

    /// @brief Calcula o histograma de uma imagem
    /// @param source A imagem fonte
    /// @param histogram Um vetor onde vai ser guardado o resultado.
    /// Deve ter 256 espaços
    /// @param channel O canal a ser contado
    /// @param luminance Se verdadeiro, calcula o histograma da luminancia
    static void Histogram(Image *source, uint32_t* histogram, Channel channel, bool luminance);
};

#endif