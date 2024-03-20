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


//largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 1280, screenHeight = 720;
SideBar *sideBar;

// mudanca na canvas para jogos
void update(float delta){
    
}

// render normal
void render(float delta)
{
    sideBar->draw();
    // imgCanvas.draw() ou algo assim

    Sleep(10);
}

void cleanup(){
    // deleta toda side bar e seus componentes
    delete sideBar;
    // delete imgcanvas
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
    // notify imgcanvas

    if(state == 0){ // mouse down
        sideBar->mouseDown();
        // notify imgcanvas
    }else if(state == 1){ // mouse up
        sideBar->mouseUp();
        // notify imgcanvas
    }
}

int main(void)
{
    sideBar = new SideBar(Vector2(0,(screenWidth/3.0f)*2), Vector2(screenWidth/3.0f, screenHeight), &screenWidth, &screenHeight);
    // imgCanvas new

    CV::init(&screenWidth, &screenHeight, "T1 - Rodrigo Appelt", false);
    CV::run();
}
