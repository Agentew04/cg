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
