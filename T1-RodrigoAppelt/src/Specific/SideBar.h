#ifndef __SIDE_BAR_H__
#define __SIDE_BAR_H__

#include "../Vector2.h"
#include "../UI/Chart.h"
#include "../UI/UIManager.h"
#include "../Image/ImageManipulation.h"
#include "../UI/Button.h"
#include "ImageCanvas.h"

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

    ImageCanvas *imgCanvas;
private:

    void submitButtons();
    void loadBmp(int n);

    /// @brief Mapa para saber quais imagens já foram carregadas
    std::map<int,bool> loadedImages;

    Vector2 pos, size, mousePos;
    UIManager* uiManager;
    Chart* histogram;
};

#endif 