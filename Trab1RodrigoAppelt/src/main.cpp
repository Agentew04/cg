/*
 * Computacao Grafica - Trabalho 1
 * Autor: Rodrigo Appelt
 *
 * Recursos Implementadas:
 *  - Carregar imagens bmp
 *  - Separar em canais: R, G, B, Y(luminância)
 *  - Espelhar(flip) na horizontal e vertical
 *  - Histograma de cada canal separado
 *    * Controlado por checkboxes abaixo do grafico
 *    * Regulagem do maximo valor do grafico por slider
 *  - Brilho
 *  - Contraste
 *  - Desfoque gaussiano
 *  - Persistencia do estado do programa em arquivos
 *    * Vai ser gerado um 'save-rodrigoappelt-t1.dat' no diretorio de execucao
 *    * Salvamento das seguintes caracteristicas:
 *      -> Imagens carregadas
 *      -> Posicao dela no canvas e sua escala
 *      -> Imagem selecionada
 *      -> Canais visiveis no histograma
 *      -> Ultima operacao realizada
 *
 * Manual:
 *  - Carregar imagem: clicar nos botoes Load 1, Load 2 ou Load 3
 *  - Mostrar canal individual na imagem de edicao:
 *    * Clicar nos botoes R, G, B ou Y
 *  - Espelhar imagem: clicar nos botoes Flip Horizontal ou Flip Vertical
 *  - Ver o histograma de cada canal da imagem selecionada:
 *    * Marcar cada checkbox abaixo do grafico
 *    * Regular o maximo p/ mais detalhes com o primeiro slider abaixo
 *  - Aplicar processamento nas imagens:
 *    * Desfoque gaussiano: ajustar o slider 'Gaussian Blur', o programa
 *      pode travar temporariamente pois e uma operacao pesada
 *    * Brilho: ajustar o slider 'Brightness'. Valores maiores vao deixar
 *      a imagem totalmente branca
 *    * Contraste: ajustar o slider 'Contrast'. Valores maiores aumentam
 *      o destaque entre as cores. Se o slider for zerado, ha 0 contraste
 *      e as cores ficam iguais(pretas).
 *
 */

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <math.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "gl_canvas2d.h"
#include "Keyboard.h"
#include "Specific/SideBar.h"
#include "Specific/ImageCanvas.h"
#include "UI/CursorManager.h"
#include "Storage/PersistentStorage.h"


//largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 1280, screenHeight = 720;
SideBar *sideBar;
ImageCanvas *imgCanvas;

// mudanca na canvas para jogos
void update(float delta){
    imgCanvas->update();
}

// render normal
void render(float delta)
{
    CV::clear(0.467f, 0.553f, 0.663f);

    sideBar->draw();
    imgCanvas->draw();


    Sleep(10);
}

void cleanup(){
    delete sideBar;
    delete imgCanvas;
    std::cout << "Cleaned" << std::endl;
    std::cout << "saida" << std::endl;
}

//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
    switch(key)
    {
        case 27:
            CV::close();
        break;
    }
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{

}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
    CursorManager::startFrame();
    Vector2 mousePos = Vector2(x,y);

    sideBar->updateMousePos(mousePos);
    imgCanvas->updateMousePos(mousePos);

    if(state == 0){ // mouse down
        sideBar->mouseDown();
        imgCanvas->mouseDown();
    }else if(state == 1){ // mouse up
        sideBar->mouseUp();
        imgCanvas->mouseUp();
    }
    CursorManager::applyCursor();
}

int main(void)
{
    PersistentStorage::load();
    imgCanvas = new ImageCanvas();
    sideBar = new SideBar(Vector2((screenWidth/4.0f)*3,0), Vector2((screenWidth/4.0f), screenHeight), &screenWidth, &screenHeight);
    sideBar->imgCanvas = imgCanvas;
    sideBar->linkImageCanvas();

    CV::init(&screenWidth, &screenHeight, "T1 - Rodrigo Appelt", false);
    CV::run();
    PersistentStorage::save();
    cleanup();
}
