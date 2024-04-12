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
#include <math.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#include "gl_canvas2d.h"

struct Bola{
    Vector2 pos;
    Vector2 direcao;
    float velocidade;
    float age;
};
std::vector<Bola> bolas;

//largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 500, screenHeight = 500;
Vector2 mousePos;

float startTetha = 0.0f;
Vector2 translation(0,0);

Vector2 origem(screenWidth/2, screenHeight/2);
float tamanhoCanhao = 75.0f;
void update(float delta){
    tamanhoCanhao = (origem - mousePos).magnitude();
    std::cout << tamanhoCanhao;
    int i=0;
    for(auto &bola : bolas){
        bola.age += delta;
        bola.pos = bola.pos + bola.direcao * bola.velocidade;
        bola.direcao.y = bola.direcao.y - delta*2;

        if(bola.pos.x > screenWidth || bola.pos.x < 0
           || bola.pos.y > screenHeight || bola.pos.y < 0){
        }
        i++;
    }
}

void render(float delta)
{
    //CV::translate(origem);
    CV::color(1,0,0);


    Vector2 direcao = Vector2(mousePos-origem).normalized();
    std::string txt = "Direcao: ("+std::to_string(direcao.x)+", "+std::to_string(direcao.y)+")";
    CV::text(Vector2(100,100), txt.c_str());

    // desenhar canhao
    float alpha = acos(Vector2::right().dot(direcao))*180/M_PI;
    txt = "Angulo: " + std::to_string(alpha) + " graus.";
    CV::text(Vector2(100,120), txt.c_str());
    CV::color(0,1,0);
    CV::line(origem, (direcao*tamanhoCanhao)+origem);

    // desenhar bolas
    for(auto &bola : bolas){
        CV::circle(bola.pos.x, bola.pos.y, 20, 50);
    }

    Sleep(10); //nao eh controle de FPS. Somente um limitador de FPS.
}

void drawSaw(){
    int steps = 15;
    float stepSize = (M_PI*2)/steps;
    float tetha = startTetha;
    float tethaAnt = tetha;
    float rTop = 125;
    float rBot = 100;
    float r = rTop;
    Vector2 lastPoint(r*cos(tetha),r*sin(tetha));
    for(int i=0; i<steps+1; i++){
        // desenhar 2 segmentos,
        //   um subindo
        //   um descendo
        float x = rTop*cos(tetha);
        float y = rTop*sin(tetha);
        CV::line(lastPoint, Vector2(x,y));
        lastPoint = Vector2(x,y);
        x = rBot*cos(tetha);
        y = rBot*sin(tetha);
        CV::line(lastPoint, Vector2(x,y));
        lastPoint = Vector2(x,y);
        tetha += stepSize;
    }
    //startTetha+=0.04;
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
        //mouse down
        Vector2 mid = Vector2(screenWidth/2, screenHeight/2);
        Vector2 direcao = (mousePos-mid).normalized();
        Bola b;
        b.pos = mid;//+direcao*75.0f;
        b.direcao = direcao;
        b.velocidade = -5*tamanhoCanhao/75.0f;
        b.age = 0;
        bolas.push_back(b);
    }else if(state == 1){
        // mouse up
    }
}

int main(void)
{
    CV::init(&screenWidth, &screenHeight, "Canvas2D - Custom Template", false);
    CV::run();
    cleanup();
}
