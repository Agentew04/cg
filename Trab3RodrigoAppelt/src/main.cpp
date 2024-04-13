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

#include "Keyboard.h"
#include "gl_canvas2d.h"

#include "Web/httplib.h"

// largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 500, screenHeight = 500;
Vector2 mousePos;

void update(float delta)
{
}

void render(float delta)
{
    Sleep(10); // nao eh controle de FPS. Somente um limitador de FPS.
}

void cleanup()
{
}

// funcao chamada toda vez que uma tecla for pressionada.
void keyboard(int key)
{
    if (key == ESC)
    {
        CV::close();
    }
    if (key == UP_ARROW)
    {
// #include "Web/picohttpclient.hpp"

        // HTTPResponse response = HTTPClient::request(HTTPClient::GET, URI("http://example.org"));
        // if(!response.success) {
        //     std::cout << "Request failed!" << std::endl;
        //     return;
        // }

        // std::cerr << "Server protocol: " << response.protocol << std::endl;
        // std::cerr << "Response code: " << response.response << std::endl;
        // std::cerr << "Response string: " << response.responseString << std::endl;

        // std::cerr << "Headers:" << std::endl;

        // for(stringMap::iterator it = response.header.begin(); it != response.header.end(); it++) {
        //     std::cerr << "\t" << it->first << "=" << it->second << std::endl;
        // }

        // std::cout << response.body << std::endl;
        
        httplib::Client cli("http://example.org");
        auto res = cli.Get("/hi");
        std::cout << res->status << std::endl;
        std::cout << res->body << std::endl;

    }
}

// funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
}

// funcao para tratamento de mouse: cliques, movimentos e arrastos
void mouse(int button, int state, int wheel, int direction, int x, int y)
{
    mousePos = Vector2(x, y);

    if (state == 0)
    {
        // mouse down
    }
    else if (state == 1)
    {
        // mouse up
    }
}

int main(void)
{
    CV::init(&screenWidth, &screenHeight, "Canvas2D - Custom Template", false);
    CV::run();
    cleanup();
}
