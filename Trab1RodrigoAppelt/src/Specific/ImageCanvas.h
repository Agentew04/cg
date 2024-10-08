#ifndef __IMAGE_CANVAS_H__
#define __IMAGE_CANVAS_H__

#include <vector>
#include <map>

#include "../UI/ImageRenderer.h"
#include "../Image/ImageManipulation.h"
#include "../UI/UIManager.h"
#include "../Math/Polygon.h"



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
    void submitImage(Image *image, int imageId);

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
    void setHistograms(Chart::Series *histR, Chart::Series *histG, Chart::Series *histB, Chart::Series *histLum);

    /// @brief Solicita uma atualizacao dos histogramas
    void updateSelectedHistograms();
    void updateBrightness(float value);
    void updateContrast(float value);
    void updateGaussian(float value);

    /// @brief Mais um callback para a ImageCanvas e SideBar se linkarem.
    /// Serve para ler o valor local de qual imagem esta selecionada.
    void readLocalSelected();

    /// @brief Taxa do maximo valor que vai ser o maximo do grafico
    float maxHistogramValueRatio = 0.5f;

    enum class Operation {
        NONE,
        EXTRACT_R,
        EXTRACT_G,
        EXTRACT_B,
        EXTRACT_Y,
        FLIP_H,
        FLIP_V,
        GAUSSIAN_BLUR,
        BRIGHTNESS,
        CONTRAST
    };
private:

    /// @brief Desenha uma moldura em volta da imagem selecionada
    void drawFrame();

    /// @brief Atualiza qual mouse deve ser mostrado
    void updateCursor();

    /// @brief Checa se a imagem selecionada esta sendo movida
    void checkScaleMovement();

    void selectImage(ImageRenderer* imgrnd);

    /// @brief recalcula o tamanho do frame da imagem selecionada
    void calculateFrames();

    Vector2 mousePos;
    bool dragging;
    Vector2 dragPivot;

    bool holdingScaleH = false;
    bool holdingScaleV = false;
    Polygon2D frameLeft, frameRight, frameTop, frameBottom;

    Chart::Series *histR, *histG, *histB, *histLum;
    bool loadedFromFileYet = false;

    
};

#endif