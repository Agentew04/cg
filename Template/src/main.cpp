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

#include "UI/UIManager.h"
#include "UI/Button.h"
#include "UI/Slider.h"
#include "UI/CheckBox.h"


//largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 500, screenHeight = 500;
UIManager *uiManager;

Vector2 mousePos;

Vector2 posObj = Vector2(400, 400);
float objRadius = 25;
bool holding = false;

void DrawMouseScreenCoords()
{
    CV::color(Vector3(1,0,0));
    char str[150];
    sprintf(str, "Mouse: (%d,%d)\nScreen: (%d,%d)a", (int)mousePos.x, (int)mousePos.y, screenWidth, screenHeight);
    CV::text(10,400, str);
}

void update(float delta){
    if(holding){
        posObj = mousePos;
    }
}

void render(float delta)
{
    CV::text(100,50,"Programa Demo Canvas2D LEFT", GLUT_BITMAP_HELVETICA_10, TextAlign::LEFT);
    CV::text(100,70,"Programa Demo Canvas2D CENTER", GLUT_BITMAP_HELVETICA_10, TextAlign::CENTER);
    CV::text(100,90,"Programa Demo Canvas2D RIGHT", GLUT_BITMAP_HELVETICA_10, TextAlign::RIGHT);

    DrawMouseScreenCoords();
    uiManager->draw();

    //draw custom obj
    CV::color(Vector3(0,0,1));
    CV::translate(0,0);
    CV::circle(posObj.x, posObj.y, objRadius, 50);

    Sleep(10); //nao eh controle de FPS. Somente um limitador de FPS.
}

void cleanup(){
    delete uiManager; // deleta toda UI e estilos tbm
}

//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
    printf("\nTecla: %d" , key);

    switch(key)
    {
        case 27:
            cleanup();
            exit(0);
        break;
    }
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
   printf("\nLiberou: %d" , key);
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
    mousePos = Vector2(x,y);

    // code for drag n drop
    if(state == 0 && posObj.distance(mousePos) <= objRadius){
        holding = true;
    }else if(state == 1){
        holding = false;
    }

    uiManager->updateMousePos(mousePos);

    if(state == 0){ // mouse down
        uiManager->mouseDown();
    }else if(state == 1){ // mouse up
        uiManager->mouseUp();
    }

    if(state != -2){
        //printa so quando clica
        printf("mouse(button %d, state %d, wheel %d, direction %d, x, y)\n", button, state, wheel, direction);
    }
}

int main(void)
{
    uiManager = new UIManager();

    ButtonStyle* btnstyle = ButtonStyle::Windows10();
    Button *btn = new Button(Vector2(50,150), Vector2(100,50), "Teste", [](){
        std::cout << "cliquei botao" << std::endl;
    });
    btn->style = btnstyle;
    Button *sqrB = new Button(Vector2(50,250), Vector2(50,50), "X", [](){
        std::cout << "segundo botao" << std::endl;
    });
    sqrB->style = btnstyle;

    Slider::Style *sldstyle = Slider::Style::Windows10();
    Slider *sld1 = new Slider(Vector2(160, 150), Vector2(100,30), 0, 10, 5, Slider::Orientation::HORIZONTAL);
    sld1->style = sldstyle;
    Slider *sld2 = new Slider(Vector2(160, 190), Vector2(150,30), 0, 10, 5, Slider::Orientation::HORIZONTAL);
    sld2->style = sldstyle;

    Checkbox::Style *chkstyle = Checkbox::Style::Windows10();
    Checkbox *chk1 = new Checkbox(Vector2(160, 230), Vector2(80,20), "Checkbox", false);
    chk1->style = chkstyle;

    uiManager->add(btn);
    uiManager->add(sqrB);
    uiManager->add(sld1);
    uiManager->add(sld2);
    uiManager->add(chk1);

    CV::init(&screenWidth, &screenHeight, "Canvas2D - Custom Template", false);
    CV::run();
}
