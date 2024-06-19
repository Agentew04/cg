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
#include <cstdarg>

#include "gl_canvas2d.h"
#include "Fonts/FontManager.h"

#include "3D/Primitive.h"
#include "3D/Perspective.h"
#include "3D/Camera.h"


//largura e altura inicial da tela . Alteram com o redimensionamento de tela.
int screenWidth = 1280, screenHeight = 720;
Vector2 mousePos;

template <typename T>
/// @brief junta inumeros vetores em um so
std::vector<T> accumulate(int count, ...){
    std::vector<T> result;
    va_list args;
    va_start(args, count);
    for(int i = 0; i < count; i++){
        std::vector<T> v = va_arg(args, std::vector<T>);
        result.insert(result.end(), v.begin(), v.end());
    }
    va_end(args);
    return result;
}

float speed = 5.0f;

std::vector<Primitive> createCrankShaft(Vector3 origin, float length, float angle){
    float axisRadius = 10;
    auto axis = Primitive::createCylinder(100, 10, axisRadius);

    std::vector<Primitive> crankShaft;
    crankShaft.push_back(axis);

    // criar 'manivela', maozinha
    auto shaft = Primitive::createCube(1);
    shaft.vertexList = P3D::scaleVector(shaft.vertexList, Vector3(1, 1, length));
    shaft.vertexList = P3D::translateVector(shaft.vertexList, Vector3(0, 0, length+axisRadius));
    shaft.vertexList = P3D::rotateVector(shaft.vertexList, Vector3(0, 0, angle));
    crankShaft.push_back(shaft);

    // agora, cria a pontinha. quase igual ao centro
    auto endPoint = Primitive::createCylinder(100, 10, axisRadius);
    endPoint.vertexList = P3D::translateVector(endPoint.vertexList, Vector3(0, 0, axisRadius+length+axisRadius));
    endPoint.vertexList = P3D::rotateVector(endPoint.vertexList, Vector3(0, 0, angle));
    crankShaft.push_back(endPoint);

    return crankShaft;
}

void drawWireframe(const Primitive& p){
    auto v = P3D::translateVector(p.vertexList, Vector3(0, 0, 5));
    auto vr = P3D::rotateVector(v, Vector3(0, 0, CV::time() * speed));
    auto vp = P3D::perspectiveProjectionVector(vr, 200);

    for(auto edge : p.edgeList){
        Vector2 v1 = vp[edge[0]];
        Vector2 v2 = vp[edge[1]];
        CV::line(v1, v2);
    }
}

std::vector<Vector3> createPiston(Vector3 origin, float length, float angle){
    std::vector<Vector3> piston;
    //
    return piston;
}

Camera3D cam;


void update(float delta){
    cam.update(delta);
}


void render()
{
    CV::translate(screenWidth/2, screenHeight/2);

    Vector3 crankshaftOrigin = Vector3::zero();
    float crankshaftLength = 100;
    auto crankShaft = createCrankShaft(crankshaftOrigin, crankshaftLength, CV::time() * speed);

    Vector3 pistonOrigin = Vector3(0, -250,0);
    float pistonLength = 100;


    // calculate end point of crankshaft
    Vector3 crankShaftEnd = crankshaftOrigin + Vector3(cos(CV::time() * speed), -sin(CV::time() * speed),0) * crankshaftLength;

    // piston Vector
    Vector3 pistonVector = (crankShaftEnd - pistonOrigin).normalized() * pistonLength;
    Vector3 pistonEnd = pistonOrigin + pistonVector;




    CV::color(1, 0, 0);
    auto cube = Primitive::createCube(5);
    cube.vertexList = P3D::rotateVector(cube.vertexList, Vector3(CV::time()));
    //cube.vertexList = cam.worldToCamera(cube.vertexList);
    cube.vertexList = P3D::translateVector(cube.vertexList, Vector3(0,0,15));
    auto vp = P3D::perspectiveProjectionVector(cube.vertexList, 200);
    for(auto edge : cube.edgeList){
        Vector2 v1 = vp[edge[0]];
        Vector2 v2 = vp[edge[1]];
        CV::line(v1, v2);
    }
    // for(auto p : crankShaft){
    //     drawWireframe(p);
    // }
    // CV::circleFill(crankshaftOrigin.toVector2(), 5, 25);
    // CV::circleFill(crankShaftEnd.toVector2(), 5, 25);
    // CV::line(crankshaftOrigin.toVector2(), crankShaftEnd.toVector2());
    // CV::color(0, 1, 0);
    // CV::circleFill(pistonOrigin.toVector2(), 5, 25);
    // CV::circleFill(pistonEnd.toVector2(), 5, 25);
    // CV::line(pistonOrigin.toVector2(), pistonEnd.toVector2());

    // CV::color(0,0,1);
    // CV::line(crankShaftEnd.toVector2(), pistonEnd.toVector2());
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
    cam.keyDown(key);
}

//funcao chamada toda vez que uma tecla for liberada
void keyboardUp(int key)
{
    cam.keyUp(key);
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
