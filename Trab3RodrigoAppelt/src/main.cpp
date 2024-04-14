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

#include "UI/ProgressRing.h"

// largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 1600, screenHeight = 900;
Vector2 mousePos;
App *app;
ObjFile objFile;

float prog = 0.0f;
void update(float delta)
{
    app->update(delta);
    prog+=delta;
}

void render()
{
    app->render();

    CV::translate(Vector2(0,0));
    CV::color(1,1,1);
    CV::text(screenWidth,25, ("FPS: " + std::to_string((int)std::round(CV::fps()))).c_str(), TextAlign::RIGHT);
    CV::translate(Vector2(200,200));
    CV::obj(objFile);
}

// funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
    if (key == ESC)
    {
        CV::close();
    }else{
        app->keyDown((Key)key);
    }
}

// funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int)
{
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

void cleanup()
{
    delete app;
}

int main(void)
{
    PersistentStorage::load("./Trab3RodrigoAppelt/saves/save.dat");

    // inicializar classes
    app = new App(&screenWidth, &screenHeight);

    objFile = ObjLoader::load("./Trab3RodrigoAppelt/images/moeda.obj");
    objFile.scale = Vector3(10,-10,10);
    CV::init(&screenWidth, &screenHeight, "Canvas2D - Custom Template", true);
    CV::run();

    cleanup();
    PersistentStorage::save();
}
