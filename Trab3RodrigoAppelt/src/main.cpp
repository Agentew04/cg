/*

Trabalho 3 (Balls Bounce) - Computacao Grafica
Autor: Rodrigo Appelt
Matricula: 202210244

Features Implementadas:
- Menu principal
- Placar de lideres online
- Menu de pausa
- Menu de game over
- Salvamento em disco da partida atual
- Efeitos de audio quando quebra bloco/perde jogo
- Explosoes com particulas quando quebra um bloco
- Progressao de niveis/fases
- Efeitos de colisao de bolinhas
- Powerups
  - Laser(Horizontal e Vertical)
  - Bola Extra
- Modo raio x
- Carregamento de arquivos 3D
- Controle de Anti-Aliasing na Canvas2D
- Controle de VSync na Canvas2D


Manual de Uso:
- Na primeira vez abrindo o aplicativo, voce deve clicar na caixa de texto
  e digitar o seu apelido e clicar no botao.
- Clique na tecla 'w' para ligar/desligar o modo raio x a qualquer momento
- Apos abrir a aplicacao, clicar em 'Jogar' ou 'Carregar'
  - se nao houver partida salva anterior, o carregar nao fara nada
- Durante o jogo, use o mouse para selecionar a direcao de lancamento
  e clique para comecar a lancar as bolinhas
- Clique ESC para pausar/despausar o jogo.
  - Clique em continuar para voltar ao jogo
  - Clique em reiniciar para desistir da partida atual
  - Clique em menu principal para salvar o estado da partida e voltar ao menu
- No jogo, se algum bloco passar da linha pontilhada, voce perde
- No menu de game over, pode ver:
  - Sua pontuacao atual
  - Sua melhor pontuacao
  - O placar de lideres online
- A sua pontuacao vai ser enviada para o placar apenas quando voce perder.
  - Se forcar o fechamento do jogo pelo CodeBlocks(e nao pelo X da janela),
  talvez alguns valores nao serao persistidos no disco tambem!
- No menu de game over, tem dois botoes para voltar ao menu
  principal e para tentar novamente

- A cada 25 blocos quebrados o jogador passa de nivel
  - Progresso indicado na barra de experiencia no topo da area de jogo
  - A cada nivel, a quantidade de moedas ganhas por bloco aumenta em 1


Observacoes:
- Caso alguma funcionalidade esteja impedindo o trabalho de
  compilar ou precise ser desabilitada, ha alguns defines
  para desabilitar funcoes especificas:

  - Em Web/WebManager.cpp:L10 (DISABLE_WEB)
    - Para tentar corrigir algum problema com relacao a link
      da httplib, tenha certeza de incluir a flag -lws2_32.
    - Nos arquivos(esquerda), botao direito no projeto
      'Canvas 2D' > Properties > Project's build options >
      > Linker Settings > Link Libraries
      - Adicionar 'ws2_32' na lista da esquerda

  - Em Misc/SoundPlayer.cpp:L9 (DISABLE_SOUND)
    - Para tentar corrigir algum problema com relacao a link
      da windows.h(PlaySound), tenha certeza de incluir a
      flag -lwinmm
    - Nos arquivos(esquerda), botao direito no projeto
      'Canvas 2D' > Properties > Project's build options >
      > Linker Settings > Link Libraries
      - Adicionar 'winmm' na lista da esquerda

  - Em gl_canvas2d.cpp:L27 (DISABLE_VSYNC)
    - Descomentando essa linha, desabilita-se o controle
    total de V-Sync do usuario.

  - Em gl_canvas2d.cpp:L28 (DISABLE_WIREFRAME)
    - Descomentando essa linha, desabilita-se o uso
    do OpenGL para desenhar os poligonos como wireframes.

  - Em gl_canvas2d.cpp:L29 (DISABLE_ANTI_ALIASING)
    - Descomentando essa linha, desabilita-se a possibilidade
    de controlar o estado do anti-aliasing da aplicacao.

- O codigo fonte atualizado sera disponibilizado em:
  https://github.com/Agentew04/cg/tree/main/Trab3RodrigoAppelt
  apos o fim da disciplina(a fim de evitar plagios)

*/

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <math.h>
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "Keyboard.h"
#include "gl_canvas2d.h"

#include "Storage/PersistentStorage.h"
#include "3D/ObjLoader.h"
#include "Fonts/FontManager.h"
#include "Misc/TaskManager.h"
#include "Misc/SoundPlayer.h"
#include "Web/WebManager.h"
#include "UI/CursorManager.h"

#include "Specific/App.h"

// largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 1280, screenHeight = 720;
Vector2 mousePos;
App *app;

void update(float delta)
{
    CursorManager::startFrame();
    app->update(delta);
}

void render()
{
    app->render();

    CV::translate(Vector2(0, 0));
    CV::color(1, 1, 1);
    CV::text(Vector2(screenWidth, 0), ("FPS: " + std::to_string((int)std::round(CV::fps()))), 20, FontName::JetBrainsMono, UIPlacement::TOP_RIGHT);
    // CV::text(screenWidth, 25, ("FPS: " + std::to_string((int)std::round(CV::fps()))).c_str(), TextAlign::RIGHT);
    TaskManager::update();
    CursorManager::applyCursor();
}

// funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
    switch (key)
    {
    case 'w':
        static bool wireframe = false;
        wireframe = !wireframe;
        CV::setWireframe(wireframe);
        break;
    default:
        app->keyDown((Key)key);
        break;
    }
}

// funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
    app->keyUp((Key)key);
}

// funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int, int state, int, int, int x, int y)
{
    mousePos = Vector2(x, y);
    app->updateMousePos(mousePos);

    if (state == 0)
    {
        app->mouseDown();
    }
    else if (state == 1)
    {
        app->mouseUp();
    }
}

void load()
{
    FontManager::load("./Trab3RodrigoAppelt/fonts/jetbrainsmono.font", FontName::JetBrainsMono);
    PersistentStorage::load("./Trab3RodrigoAppelt/saves/save.dat");
    ObjLoader::load("./Trab3RodrigoAppelt/models/moeda.3d", "coin");
    ObjLoader::load("./Trab3RodrigoAppelt/models/logo.3d", "logo");
    ObjLoader::load("./Trab3RodrigoAppelt/models/trophy.3d", "trophy");
    ObjLoader::load("./Trab3RodrigoAppelt/models/music.3d", "music");
    ObjLoader::load("./Trab3RodrigoAppelt/models/pause.3d", "pause");
    ObjLoader::load("./Trab3RodrigoAppelt/models/star.3d", "star");
    ObjLoader::load("./Trab3RodrigoAppelt/models/collisionParticle.3d", "collisionParticle");
    ObjLoader::load("./Trab3RodrigoAppelt/models/powerupBall.3d", "powerupBall");
    ObjLoader::load("./Trab3RodrigoAppelt/models/powerupLaser.3d", "powerupLaser");
    SoundPlayer::load("./Trab3RodrigoAppelt/audio/ballHit1.wav", "ballHit");
    SoundPlayer::load("./Trab3RodrigoAppelt/audio/ballHit2.wav", "ballExit");
    SoundPlayer::load("./Trab3RodrigoAppelt/audio/laserZap.wav", "laserZap");
    app = new App(&screenWidth, &screenHeight);
}

/// @brief Libera os recursos alocados pelo aplicativo
void cleanup()
{
    ObjLoader::free();
    delete app;
    PersistentStorage::save();
    WebManager::free();
    CursorManager::freeResources();
}

int main(void)
{
    load();
    CV::init(
        /* Screen Width*/&screenWidth,
        /* Screen Height*/&screenHeight,
        /* Title*/ "Bolas Saltitantes do Rodrigo Appelt",
        /*anti-aliasing: */ true,
        /*vsync: */ true);
    CV::run();
    cleanup();
}
