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

#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include "gl_canvas2d.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Matrix.h"

struct Square{
    Vector2 vertices[4];

    Square(Vector2 v0, Vector2 v1, Vector2 v2, Vector2 v3){
        vertices[0] = v0;
        vertices[1] = v1;
        vertices[2] = v2;
        vertices[3] = v3;
    }

    Square(){
        vertices[0] = Vector2(0,0);
        vertices[1] = Vector2(0,0);
        vertices[2] = Vector2(0,0);
        vertices[3] = Vector2(0,0);
    }

    Square(std::vector<Vector2> v){
        for(int i=0; i<4; i++){
            vertices[i] = v[i];
        }
    }
};

//largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 500, screenHeight = 500;
Vector2 mousePos;

void update(float delta){

}



Square translateSquare(Square sq, Vector2 offset){
    Square newSq;
    for(int i=0; i<4; i++){
        newSq.vertices[i] = sq.vertices[i] + offset;
    }
    return newSq;
}

void drawAxis(){
    CV::color(0,0,0);
    // draw X
    CV::line(Vector2(-screenWidth/2,0), Vector2(screenWidth/2,0));
    // draw Y
    CV::line(Vector2(0,-screenHeight/2), Vector2(0,screenHeight/2));
}

Square rotateSquare(Square sq, float radians){
    Square newSq;
    for(int i=0;i<4;i++){
        Vector2 v = sq.vertices[i];
        float x = std::cos(radians)*v.x - std::sin(radians)*v.y;
        float y = std::sin(radians)*v.x + std::cos(radians)*v.y;
        newSq.vertices[i] = Vector2(x,y);
    }
    return newSq;
}

Square scaleSquare(Square sq, Vector2 factor){
    Square newSq;
    for(int i=0; i<4; i++){
        newSq.vertices[i] = sq.vertices[i].multiply(factor);
    }
    return newSq;
}

void drawSquare(Square sq){
    for(int i=0; i<4; i++){
        CV::line(sq.vertices[i], sq.vertices[(i+1)%4]);
    }
}

void cleanup(){
}

//funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
    printf("\nTecla: %d" , key);

    switch(key)
    {
        case 27:
            CV::close();
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
    mousePos = Vector2(x,y);
    if(state == 0){
    }else if(state == 1){
        // mouse up
    }
}

void p1_2023_1(){
    float x = 150, y = 100;
    float l = 50;
    float alpha = std::atan(y/x);
    drawAxis();

    Square original = translateSquare(
        rotateSquare(
            Square(std::vector<Vector2>{ {0,0}, {l,0}, {l,l}, {0,l}}),
            alpha
        ), Vector2(x,y)
    );
    CV::color(1,0,0);
    drawSquare(original);

    // mover pra origem
    Square translated = translateSquare(original, Vector2(-x,-y));
    CV::color(0,1,0);
    drawSquare(translated);

    // endireitar
    Square rotated = rotateSquare(translated, -alpha);
    CV::color(0,0,1);
    drawSquare(rotated);

    // escalar
    Square scaled = scaleSquare(rotated, Vector2(0.6f, 0.6f));
    CV::color(1,0,0);
    drawSquare(scaled);

    // volta pra posicao original

    Square final = translateSquare(scaled, Vector2(x,y));
    CV::color(0,1,0);
    drawSquare(final);
}

void p1_2022_1(){
    float l = 50;
    float x = 100, y = 100;
    float alpha = std::atan(y/(-x)) + M_PI - M_PI/2;
    drawAxis();

    Square original = Square(std::vector<Vector2>{ {-l,-l}, {0,-l}, {0,0}, {-l,0}});
    CV::color(1,0,0);
    drawSquare(original);

    // mover pra origem
    Square translated = translateSquare(original, Vector2(l,l));
    CV::color(0,1,0);
    drawSquare(translated);

    // endireitar
    Square rotated = rotateSquare(translated, alpha);
    CV::color(0,0,1);
    drawSquare(rotated);

    // escalar
    Square scaled = scaleSquare(rotated, Vector2(0.3f, 0.3f));
    CV::color(1,0,0);
    drawSquare(scaled);

    // volta pra posicao original
    Square final = translateSquare(scaled, Vector2(x,0));
    CV::color(0,1,0);
    drawSquare(final);
}

void p1_2021_1(){
    
}
void render()
{
    CV::clear(1,1,1);
    CV::translate(screenWidth/2, screenHeight/2);

    // p1_2023_1();
    p1_2022_1();


    Sleep(10); //nao eh controle de FPS. Somente um limitador de FPS.
}

int main(void)
{
    CV::init(&screenWidth, &screenHeight, "Canvas2D - Custom Template", false, true);
    CV::run();
    cleanup();
}
