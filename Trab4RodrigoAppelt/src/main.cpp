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

float speed = 2.0f;

std::vector<Primitive> createCrankShaft(Vector3 origin, float length, float angle){
    float axisRadius = 10;
    float axisHeight = 10;
    auto axis = Primitive::createCylinder(15, axisHeight, axisRadius);
    axis.vertexList = P3D::rotateVector(axis.vertexList, Vector3(0,  3.1415f/2, 0));

    std::vector<Primitive> crankShaft;
    crankShaft.push_back(axis);

    // criar 'manivela', maozinha
    auto shaft = Primitive::createCube(1);
    shaft.vertexList = P3D::scaleVector(shaft.vertexList, Vector3(1, 1, length-axisRadius));
    shaft.vertexList = P3D::translateVector(shaft.vertexList, Vector3(0, 0, (length-axisRadius)/2+axisRadius));
    shaft.vertexList = P3D::scaleVector(shaft.vertexList, Vector3(axisHeight, axisHeight, 1));
    shaft.vertexList = P3D::rotateVector(shaft.vertexList, Vector3(angle, 0, 0));
    crankShaft.push_back(shaft);

    // agora, cria a pontinha. quase igual ao centro
    auto endPoint = Primitive::createCylinder(15, axisHeight, axisRadius);
    endPoint.vertexList = P3D::rotateVector(endPoint.vertexList, Vector3(0,  3.1415f/2, 0));
    endPoint.vertexList = P3D::translateVector(endPoint.vertexList, Vector3(0, 0, length+axisRadius));
    endPoint.vertexList = P3D::rotateVector(endPoint.vertexList, Vector3(angle, 0, 0));
    crankShaft.push_back(endPoint);

    return crankShaft;
}


std::vector<Primitive> createPiston(Vector3 origin, float length, Vector3 crankshaftEnd){
    Vector3 pistonVector = (crankshaftEnd - origin).normalized();
    float angle = acos(Vector3(0,0,1).dot(pistonVector));
    std::cout << "angle: " << angle * (180/3.1415f) << std::endl;
    auto piston = Primitive::createCube(1);
    piston.vertexList = P3D::translateVector(piston.vertexList, Vector3(0, 0, -0.5f));
    piston.vertexList = P3D::scaleVector(piston.vertexList, Vector3(10, 10, length));
    piston.vertexList = P3D::rotateVector(piston.vertexList, Vector3(angle, 0, 0));
    piston.vertexList = P3D::translateVector(piston.vertexList, Vector3(origin.x, origin.z, -origin.y));
    return { piston };
}

Camera3D cam = Camera3D(Vector3(-200, 0, 0));
Vector3 sun(-1,1,-1);

void draw(const Primitive& p, const Camera3D& cam, bool wireframe = true){
    auto vc = cam.worldToCamera(p.vertexList);
    auto cameraSun = cam.worldToCamera(sun);

    // para nao wireframe(preenchido), nos ordenamos as faces de acordo com
    // a distancia delas ate a camera
    auto vp = P3D::perspectiveProjectionVector(vc, cam.getD());

    if(wireframe){
        for(auto edge : p.edgeList){
            Vector2 v1 = vp[edge[0]];
            Vector2 v2 = vp[edge[1]];
            if(cam.isOnFrustumCS(vc[edge[0]]) && cam.isOnFrustumCS(vc[edge[1]])){
                CV::line(v1, v2);
            }
        }
    }else{
        auto sortedFaces = p.faceList;
        P3D::sortFaces(vc, sortedFaces);
        // temos que transformar as normais tambem
        auto csNormals = cam.worldToCamera(p.normalList);
        std::vector<bool> drawFaces = std::vector<bool>(sortedFaces.size(), true);

        int vertexFaceCount = sortedFaces[0].size();
        bool accumulateFaces = vertexFaceCount == 3 || vertexFaceCount == 4;
        for(int i = 0; i < sortedFaces.size(); i++){
            auto face = sortedFaces[i];
            bool drawFace = true;
            for(auto v: face){
                if(!cam.isOnFrustumCS(vc[v])){
                    drawFace = false;
                    break;
                }
            }
            if(!accumulateFaces && drawFace){
                glBegin(GL_POLYGON);
                Vector3 normal = csNormals[i];
                for(auto v: face){
                    Vector2 p = vp[v];
                    float shade = normal.normalized().dot(cameraSun.normalized());
                    glColor3f(shade, shade, shade);
                    glVertex2f(p.x, p.y);
                }
                glEnd();
            }
        }
        if(accumulateFaces){
            std::cout << "accum" << std::endl;
            glBegin(vertexFaceCount == 3 ? GL_TRIANGLES : GL_QUADS);
            for(int i = 0; i < sortedFaces.size(); i++){
                if(drawFaces[i]){
                    Vector3 normal = csNormals[i];
                    auto face = sortedFaces[i];
                    for(auto v: face){
                        Vector2 p = vp[v];
                        float shade = normal.normalized().dot(cameraSun.normalized());
                        glColor3f(shade, shade, shade);
                        glVertex2f(p.x, p.y);
                    }
                }
            }

            glEnd();
        }
    }
    
}


void update(float delta){
    cam.update(delta);
}


void render()
{
    CV::translate(screenWidth/2, screenHeight/2);

    Vector3 crankshaftOrigin = Vector3::zero();
    float crankshaftLength = 100;
    Vector3 crankShaftEnd = crankshaftOrigin + Vector3(cos(CV::time() * speed), -sin(CV::time() * speed),0) * crankshaftLength;
    auto crankShaft = createCrankShaft(crankshaftOrigin, crankshaftLength, CV::time() * speed);

    Vector3 pistonOrigin = Vector3(0,-250,0);
    float pistonLength = 100;
    Vector3 pistonVector = (crankShaftEnd - pistonOrigin).normalized() * pistonLength;
    Vector3 pistonEnd = pistonOrigin + pistonVector;
    auto piston = createPiston(pistonOrigin, pistonLength, crankShaftEnd);

    CV::color(1, 0, 0);
    for(auto &p : crankShaft){
        draw(p, cam);
    }

    CV::color(0,1,0);
    for(auto &p : piston){
        draw(p, cam);
    }

    // draw axis for reference
    auto xaxis = Primitive::createCylinder(15, 100, 5);
    xaxis.vertexList = P3D::rotateVector(xaxis.vertexList, Vector3(0,  3.1415f/2, 0));
    auto yaxis = Primitive::createCylinder(15, 100, 5);
    yaxis.vertexList = P3D::rotateVector(yaxis.vertexList, Vector3(3.1415f/2, 0, 0));
    auto zaxis = Primitive::createCylinder(15, 100, 5);
    CV::color(1,0,0);
    draw(xaxis, cam);
    CV::color(0,1,0);
    draw(yaxis, cam);
    CV::color(0,0,1);
    draw(zaxis, cam);

    // CV::circleFill(crankshaftOrigin.toVector2(), 5, 25);
    // CV::circleFill(crankShaftEnd.toVector2(), 5, 25);
    // CV::line(crankshaftOrigin.toVector2(), crankShaftEnd.toVector2());
    // CV::color(0, 1, 0);
    // CV::circleFill(pistonOrigin.toVector2(), 5, 25);
    // CV::circleFill(pistonEnd.toVector2(), 5, 25);
    // CV::line(pistonOrigin.toVector2(), pistonEnd.toVector2());

    // CV::color(0,0,1);
    // CV::line(crankShaftEnd.toVector2(), pistonEnd.toVector2());
    //Sleep(10); //nao eh controle de FPS. Somente um limitador de FPS.
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
    cam.setD(640); // eh mais ou menos 90 graus de fov horizontal

    CV::init(&screenWidth, &screenHeight, "Canvas2D - Custom Template", true, true);
    CV::run();
    cleanup();
}
