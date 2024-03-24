#ifndef __SIDE_BAR_H__
#define __SIDE_BAR_H__

#include "../Vector2.h"
#include "../UI/Chart.h"
#include "../UI/UIManager.h"
#include "../Image/ImageManipulation.h"
#include "../UI/Button.h"

class SideBar {
public:

    /// @brief Cria uma nova instancia de uma sidebar. controla todos
    /// os botoes, a mini imagem e o histograma
    /// @param pos a posicao da sidebar
    /// @param size tamanho
    /// @param scrW ponteiro para largura da tela
    /// @param scrH ponteiro para altura
    SideBar(Vector2 pos, Vector2 size, int *scrW, int *scrH);

    /// @brief Libera todos os recursos usados pela sidebar
    ~SideBar();

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
private:

    void submitButtons();
    void loadBmp(int n);
    void drawFrame();

    /// @brief Organiza as imagens na ordem de prioridade
    /// dentro do uiManager;
    void sortImages();

    std::vector<Button*> buttons;

    /// @brief Vetor dos objetos de imagens. Tem posicao e tamanho.
    /// Ele esta ordenado na ordem de prioridade de click e de render.
    std::vector<ImageRenderer*> imgrenderers;

    /// @brief Mapa para saber quais imagens já foram carregadas
    std::map<int,bool> loadedImages;

    /// @brief Vetor com todas as imagens carregadas
    std::vector<Image*> images;

    /// @brief So um atalho pra achar o img renderer 
    /// de uma imagem sem ter q fazer uma busca toda hora
    std::map<Image*, ImageRenderer*> imgToRenderer;

    Image* selectedImage = nullptr;

    Vector2 pos, size, mousePos;
    UIManager* uiManager;
    Chart* histogram;
};

#endif 