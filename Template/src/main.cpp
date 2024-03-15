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
//  Instruções:
//	  Para alterar a animacao, digite numeros entre 1 e 3
// *********************************************************************/

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include <math.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "gl_canvas2d.h"

#include "Bola.h"
#include "Relogio.h"
#include "Botao.h"

#include "UI/ButtonManager.h"
#include "UI/Button.h"

//largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 500, screenHeight = 500;
ButtonManager buttonManager;

int mouseX, mouseY; //variaveis globais do mouse para poder exibir dentro da render().

Vector2 posObj = Vector2(screenWidth/2, screenHeight/2);
float objRadius = 25;
bool holding = false;

void DrawMouseScreenCoords()
{
    CV::color(Vector3(1,0,0));
    char str[150];
    sprintf(str, "Mouse: (%d,%d)\nScreen: (%d,%d)a", mouseX, mouseY, screenWidth, screenHeight);
    CV::text(10,300, str);
}









//funcao chamada continuamente. Deve-se controlar o que desenhar por meio de variaveis globais
//Todos os comandos para desenho na canvas devem ser chamados dentro da render().
//Deve-se manter essa função com poucas linhas de codigo.
void render()
{
    CV::text(100,50,"Programa Demo Canvas2D LEFT", GLUT_BITMAP_HELVETICA_10, TextAlign::LEFT);
    CV::text(100,70,"Programa Demo Canvas2D CENTER", GLUT_BITMAP_HELVETICA_10, TextAlign::CENTER);
    CV::text(100,90,"Programa Demo Canvas2D RIGHT", GLUT_BITMAP_HELVETICA_10, TextAlign::RIGHT);

    DrawMouseScreenCoords();
    buttonManager.draw();

    //draw custom obj
    CV::color(Vector3(0,0,1));

    if(holding){
        posObj = Vector2(mouseX, mouseY);
    }

    CV::translate(0,0);
    CV::circle(posObj.x, posObj.y, objRadius, 50);

    Sleep(10); //nao eh controle de FPS. Somente um limitador de FPS.
}

//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
   printf("\nTecla: %d" , key);

   switch(key)
   {
      case 27:
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
    // state 0 = mouse down
    // state 1 = mouse up
   mouseX = x; //guarda as coordenadas do mouse para exibir dentro da render()
   mouseY = y;
   Vector2 mouse = Vector2(x,y);

    if(state == 0 && posObj.distance(mouse) <= objRadius){
        holding = true;
    }else if(state == 1){
        holding = false;
    }

    buttonManager.updateMousePos(mouse);

    if(state != -2){
        //printa so quando clica
        printf("\nmouse %d %d %d %d %d %d", button, state, wheel, direction,  x, y);
    }
}



int main(void)
{
    buttonManager = ButtonManager();
    Button *btn = new Button(Vector2(50,150), Vector2(100,50), "Teste", false, [](){
        std::cout << "cliquei botao" << std::endl;
    });
    std::cout << "setando style" << std::endl;
    ButtonStyle style = ButtonStyle::Windows10();
    std::cout << style.backgroundColor[btn.state] << std::endl;
    std::cout << "style setado" << std::endl;

    buttonManager.registerButton(btn);

    CV::init(&screenWidth, &screenHeight, "Titulo da Janela: Canvas 2D - Pressione 1, 2, 3");
    CV::run();
}
