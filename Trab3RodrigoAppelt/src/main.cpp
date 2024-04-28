/*********************************************************************
// Canvas para desenho, criada sobre a API OpenGL. Nao eh necessario conhecimentos de OpenGL para usar.
//  Autor: Cesar Tadeu Pozzer
//         05/2024
//
//  Pode ser utilizada para fazer desenhos, animacoes, e jogos simples.
//  Tem tratamento de mouse e teclado
//  Estude o OpenGL antes de tentar compreender o arquivo gl_canvas.cpp
//
//  Versao 2.0
//
// *********************************************************************/

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
#include "Specific/App.h"
#include "3D/ObjLoader.h"
#include "Fonts/FontManager.h"
#include "Misc/TaskManager.h"
#include "Misc/SoundPlayer.h"


// largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 1280, screenHeight = 720;
Vector2 mousePos;
App *app;

void update(float delta)
{
    app->update(delta);
}

void render()
{
    app->render();

    CV::translate(Vector2(0,0));
    CV::color(1,1,1);
    CV::text(screenWidth,25, ("FPS: " + std::to_string((int)std::round(CV::fps()))).c_str(), TextAlign::RIGHT);
    TaskManager::update();
}

// funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
    switch (key){
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

void load(){
    PersistentStorage::load("./Trab3RodrigoAppelt/saves/save.dat");
    ObjLoader::load("./Trab3RodrigoAppelt/models/moeda.3d", "coin");
    ObjLoader::load("./Trab3RodrigoAppelt/models/logo.3d", "logo");
    ObjLoader::load("./Trab3RodrigoAppelt/models/trophy.3d", "trophy");
    ObjLoader::load("./Trab3RodrigoAppelt/models/music.3d", "music");
    ObjLoader::load("./Trab3RodrigoAppelt/models/pause.3d", "pause");
    ObjLoader::load("./Trab3RodrigoAppelt/models/star.3d", "star");
    ObjLoader::load("./Trab3RodrigoAppelt/models/powerupBall.3d", "powerupBall");
    SoundPlayer::load("./Trab3RodrigoAppelt/audio/ballHit1.wav", "ballHit");
    SoundPlayer::load("./Trab3RodrigoAppelt/audio/ballHit2.wav", "ballExit");
    app = new App(&screenWidth, &screenHeight);
}

void cleanup()
{
    ObjLoader::free();
    delete app;
    PersistentStorage::save();
}

int main(void)
{
    load();
    CV::init(&screenWidth, &screenHeight, "Bolas Saltitantes do Rodrigo Appelt", /*anti-aliasing: */true, /*vsync: */true);
    CV::run();
    cleanup();
}
