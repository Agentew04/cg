#ifndef __IMAGE_CANVAS_H__
#define __IMAGE_CANVAS_H__

#include <vector>
#include <map>

#include "../UI/ImageRenderer.h"
#include "../Image/ImageManipulation.h"
#include "../UI/UIManager.h"



/// @brief Classe que controla todo o comportamento
/// de renderizacao, selecao, movimento
/// e manipulacao das imagens na tela.
class ImageCanvas {
public:

    ImageCanvas();
    ~ImageCanvas();

    /// @brief Lista com todos os imgRenderers na canvas.
    /// Representa os elementos na tela.
    /// Garantido de estar na ordem de prioridade de click e render.
    std::vector<ImageRenderer*> imgrenderers;

    /// @brief So um atalho pra achar o img renderer 
    /// de uma imagem sem ter q fazer uma busca toda hora
    std::map<Image*,ImageRenderer*> imgToRenderer;

    ImageRenderer* selectedImageRenderer;

    ImageRenderer* editionImageRenderer;

    void update();

    /// @brief Desenha a sidebar na tela
    void draw();

    /// @brief notifica a sidebar de um movimento no mouse.
    /// usado para o efeito de hover
    /// @param mousePos a nova pos do mouse
    void updateMousePos(Vector2 mousePos);

    /// @brief evento de quando o usuario clica no mouse
    void mouseDown();

    /// @brief evento de quando o usuario solta o clique do mouse
    void mouseUp();

    /// @brief Adiciona uma imagem na canvas
    /// @param image a imagem a ser adicionada
    void submitImage(Image *image);

    /// @brief Sinaliza para mostrar apenas o canal definido
    ///da imagem selecionada na quarta imagem
    /// @param channel o canal a ser mostrado
    void requestChannel(ImageManipulation::Channel channel, bool luminance = false);

    /// @brief Sinaliza para flipar a imagem selecionada e mostrar o resultado
    /// na quarta imagem
    void requestFlip(bool vertical);

    /// @brief Define os vetores onde o histograma de cada imagem sera guardado
    /// @param histR O histograma do canal vermelho
    /// @param histG O histograma do canal verde
    /// @param histB O histograma do canal azul
    /// @param histLum O histograma da luminancia
    void setHistograms(uint32_t *histR, uint32_t *histG, uint32_t *histB, uint32_t *histLum);
private:

    /// @brief Desenha uma moldura em volta da imagem selecionada
    void drawFrame();

    void selectImage(ImageRenderer* imgrnd);

    Vector2 mousePos;
    bool dragging;
    Vector2 dragPivot;

    uint32_t *histR, *histG, *histB, *histLum;
};

#endif