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

#include "gl_canvas2d.h"
#include "Keyboard.h"
#include "Specific/SideBar.h"
#include "Specific/ImageCanvas.h"


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
    sideBar->draw();
    imgCanvas->draw();

    Sleep(10);
}

void cleanup(){
    // deleta toda side bar e seus componentes
    delete sideBar;
    delete imgCanvas;
}

//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
    switch(key)
    {
        case ESC:
            cleanup();
            std::cout << "Cleaned" << std::endl;
            exit(0);
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
    Vector2 mousePos = Vector2(x,y);

    // TODO mover para um ImageCanvas.cpp!!
    // code for drag n drop
    // if(state == 0 && posObj.distance(mousePos) <= objRadius){
    //     holding = true;
    // }else if(state == 1){
    //     holding = false;
    // }

    sideBar->updateMousePos(mousePos);
    imgCanvas->updateMousePos(mousePos);

    if(state == 0){ // mouse down
        sideBar->mouseDown();
        imgCanvas->mouseDown();
    }else if(state == 1){ // mouse up
        sideBar->mouseUp();
        imgCanvas->mouseUp();
    }
}

int main(void)
{
    imgCanvas = new ImageCanvas();
    sideBar = new SideBar(Vector2((screenWidth/4.0f)*3,0), Vector2((screenWidth/4.0f), screenHeight), &screenWidth, &screenHeight);
    sideBar->imgCanvas = imgCanvas;
    sideBar->linkImageCanvas();

    CV::init(&screenWidth, &screenHeight, "T1 - Rodrigo Appelt", false);
    CV::run();
}
