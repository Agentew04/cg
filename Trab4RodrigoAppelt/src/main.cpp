/*

Computacao Grafica
Trabalho 4 - Simulacao de Motor

Aluno: Rodrigo Appelt

Requisitos implementados:
  - Pistao, Cilindro e Virabrequim
  - Engrenagens
  - Visualizacao Livre em Perspectiva
  - Visualizacao Ortografica
    - Topo
    - Frente
    - Lado

Extras implementados:
  - Mudanca na velocidade (RPM) da simulacao
  - Alternar visibilidade de cada peca da simulacao
    - Pistao
    - Cilindro
    - Virabrequim
    - Engrenagens
    - Eixo Cardan
  - Ocultar pecas aumenta o desempenho(em wireframe e iluminacao por pixel)
  - Eixo Cardan
    - Rotacao de 0 a 45 graus
  - Iluminacao por pixel
  - Opcao de mostrar o Depth Buffer(Z-Buffer)
  - Opcao de mostrar as normais interpoladas de cada face
  - Controle da iluminacao ambiente
  - Upscale de ate 8x para acelerar a renderizacao

Como usar:
  1. Na esquerda temos a visualizacao principal
    1.1. No modo 'Perspectiva Livre', pode controlar a camera com:
      - 'W', 'A', 'S', 'D' para movimento horizontal
      - Espaco ou 'E' para subir verticalmente
      - 'Q' ou 'C' ou ShiftEsquerdo para descer verticalmente
      - Setas do teclado para rotacionar a camera
    1.2 Em cima na esquerda, temos informacao sobre o frame atual.
      - A media de FPS dos ultimos 60 frames
      - O tempo de CPU gasto para renderizar o ultimo frame
  2. Na direita temos a barra de controle
    2.1. No primeiro slider, abaixo de "RPM: 30.00", voce
         pode controlar a velocidade da simulacao.
    2.2. Abaixo da label "Camera", existem 4 botoes que controlam
         qual camera vai ser visualiazda na tela principal.
         - "Perspectiva Livre" - Camera 3D que pode ser controlada
           pelo usuario
         - "Ortografica Frente" - Camera 2D que olha para o plano 
         XZ (o sistema de coordenadas usado eh direito com o Z para cima)
         - "Ortografica Lado" - Camera 2D que olha para o plano YZ
         - "Ortografica Topo" - Camera 2D que olha para o plano XY
    2.3. Abaixo da label "Visibilidade", existem 5 checkboxes que controlam
         a visibilidade de cada peca da simulacao. Ocultar pecas pode aumentar
         o desempenho em maquinas mais lentas. Os mais pesados eh o eixo cardan
         e as engrenagens
         - "Pistao" - Mostra ou esconde o pistao
         - "Cilindro" - Mostra ou esconde o cilindro
         - "Virabrequim" - Mostra ou esconde o virabrequim
         - "Engrenagens" - Mostra ou esconde as engrenagens
         - "Eixo Cardan" - Mostra ou esconde o eixo cardan
    2.4. Abaixo da label "Eixo Cardan", ha um slider para controlar a rotacao
         do eixo cardan. O valor vai de 0 ate 45 graus
    2.5. Abaixo da label "Rasterizador", estao as opcoes relacionadas a
         iluminacao por pixel
         2.5.1. Checkbox "Rasterizador" - Liga ou desliga a iluminacao por pixel.
                Eh muito pesado! +-500ms/2fps com um I5-7400
         2.5.2. Botao "Mostrar Color Buffer" - Habilitado por padrao. Mostra
                as cores finais da cena com iluminacao.
         2.5.3. Botao "Mostrar Depth Buffer" - Mostra o ZBuffer do frame.
                Branco eh [10000,infinito)
                Preto eh 0
         2.5.4. Botao "Mostrar Normais Interpoladas" - Mostra o valor da normal
                de cada face pixel a pixel.
         2.5.5. Slider "Intensidade Iluminacao Ambiente" - Controla a intensidade
                da iluminacao ambiente. O valor vai de 0(sombras pretas) a 
                1(tudo eh iluminado igualmente).
         2.5.6. Slider "Upscale" - Controla o upscale da tela. O valor vai de 1
                a 8x. Aumentar o upscale pode acelerar a renderizacao, mas
                diminui a qualidade da imagem.
*/

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <math.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <iomanip>

#include "gl_canvas2d.h"
#include "Fonts/FontManager.h"
#include "UI/CursorManager.h"

#include "Specific/Manager.h"
#include "3D/Buffer.h"


//largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 1280, screenHeight = 720;
Vector2 mousePos;

Manager mngr = Manager(&screenWidth, &screenHeight);


void update(float delta){
    mngr.update(delta);
}

Buffer *buffer = nullptr;

void render()
{
    mngr.render();
    CV::translate(Vector2::zero());
    CV::color(0,0,0);
    int fps = (int)std::round(CV::fps());
    float cpuMs = CV::delta() * 1000;
    std::stringstream ss;
    ss << std::fixed << std::setprecision(1) << cpuMs;
    std::string header = "FPS: " + std::to_string(fps) + "; CPU: " + ss.str() + "ms";
    CV::text(Vector2(5), header, 20, FontName::JetBrainsMono, UIPlacement::TOP_LEFT);
}

void cleanup(){
    CursorManager::freeResources();
    ObjLoader::free(); // libera os recursos do FontManager
}

//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
    if(key==27){
        CV::close();
    }
    mngr.keyDown(key);
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
    mngr.keyUp(key);
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
    CursorManager::startFrame();
    mousePos = Vector2(x,y);
    mngr.updateMousePos(mousePos);

    if(state == 0){
        mngr.mouseDown();
    }else if(state == 1){
        mngr.mouseUp();
    }
    CursorManager::applyCursor();
}

int main(void)
{
    FontManager::load("./Template/assets/fonts/jetbrainsmono.font", FontName::JetBrainsMono);
    buffer = new Buffer(screenWidth, screenHeight, 3);
    CV::init(&screenWidth, &screenHeight, "Rodrigo Appelt - T4 - \"Motor\"", true, true);
    CV::run();
    cleanup();
}
