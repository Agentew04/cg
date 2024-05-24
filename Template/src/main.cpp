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
#include "Fonts/FontManager.h"


//largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 500, screenHeight = 500;
Vector2 mousePos;

void update(float delta){

}

void render()
{
    CV::clear(1,1,1);
    CV::translate(0,0);
    CV::color(1,0,0);
    // CV::obj(ObjLoader::get("./Template/assets/fonts/jetbrainsmono.font", "a"), 
    //     Vector2(300,300),
    //     Vector2(50,50));
    // CV::setWireframe(true);
    // CV::obj(ObjLoader::get("moeda"), 
    //     Vector2(300,300),
    //     Vector2(50,50));
    std::string msg = "H#llo W0rld!\nNew Lin*.";
    Vector2 origin = Vector2(300,300);
    float pt = 40;

    // CV::color(0,1,0);
    // CV::circleFill(origin, 5, 10);
    float textWidth = FontManager::getTextWidth(
        FontName::JetBrainsMono, 
        msg, 
        pt);
    float textHeight = FontManager::getTextHeight(
        FontName::JetBrainsMono, 
        msg, 
        pt);

    CV::color(1,0,0);
    CV::text(
        origin,
        msg,
        FontName::JetBrainsMono,
        pt,
        UIPlacement::CENTER
    );
    CV::color(0,0,1);
    CV::line(origin, origin + Vector2(textWidth, 0));
    CV::line(origin, origin + Vector2(0, textHeight));

    Sleep(10); //nao eh controle de FPS. Somente um limitador de FPS.
}

void cleanup(){

}

//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
    if(key==27){
        CV::close();
    }
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
}

//funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
    mousePos = Vector2(x,y);

    if(state == 0){
        // mouse down
    }else if(state == 1){
        // mouse up
    }
}

int main(void)
{
    FontManager::load("./Template/assets/fonts/jetbrainsmono.font", FontName::JetBrainsMono);

    CV::init(&screenWidth, &screenHeight, "Canvas2D - Custom Template", true, true);
    CV::run();
    cleanup();
}
