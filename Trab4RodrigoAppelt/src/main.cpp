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

void drawAxis(const Camera3D& cam){
    auto xaxis = Primitive::createCylinder(15, 1, 1);
    xaxis.vertexList = P3D::translateVector(xaxis.vertexList, Vector3(0,0,0.5));
    xaxis.vertexList = P3D::scaleVector(xaxis.vertexList, Vector3(1,1,10));
    xaxis.vertexList = P3D::rotateVector(xaxis.vertexList, Vector3(0,  3.1415f/2, 0));
    auto yaxis = Primitive::createCylinder(15, 1, 1);
    yaxis.vertexList = P3D::translateVector(yaxis.vertexList, Vector3(0,0,0.5));
    yaxis.vertexList = P3D::scaleVector(yaxis.vertexList, Vector3(1,1,10));
    yaxis.vertexList = P3D::rotateVector(yaxis.vertexList, Vector3(3.1415f/2, 0, 0));
    auto zaxis = Primitive::createCylinder(15, 1, 1);
    zaxis.vertexList = P3D::translateVector(zaxis.vertexList, Vector3(0,0,0.5));
    zaxis.vertexList = P3D::scaleVector(zaxis.vertexList, Vector3(1,1,10));
    CV::color(1,0,0);
    draw(xaxis, cam);
    CV::color(0,1,0);
    draw(yaxis, cam);
    CV::color(0,0,1);
    draw(zaxis, cam);
}

void update(float delta){
    cam.update(delta);
}



void render()
{
    CV::translate(screenWidth/2, screenHeight/2);

    SimulationValues simval;

    simval.rpm = 15;
    simval.crankshaftOrigin = Vector3::zero();
    simval.crankshaftLength = 100;
    simval.crankshaftAngle = CV::time() * simval.rpm * RPM_TO_RADS;
    simval.crankshaftDirection = Vector3(0, -sin(simval.crankshaftAngle), cos(simval.crankshaftAngle));
    simval.crankshaftEnd = simval.crankshaftOrigin + (simval.crankshaftDirection * simval.crankshaftLength);
    auto crankShaft = createCrankShaft(simval);

    simval.spacePistonCrankshaft = 10;

    simval.pistonArmLength = simval.crankshaftLength*2 + simval.spacePistonCrankshaft;
    simval.pistonBaseLength = simval.pistonArmLength; // a base vai ficar com um pouco de espaco sobrando em cima. intencional!
    simval.pistonOrigin = simval.crankshaftOrigin;
    simval.pistonOrigin.z += simval.crankshaftLength + simval.spacePistonCrankshaft + simval.pistonArmLength; 
    simval.pistonDirection = (simval.crankshaftEnd - simval.pistonOrigin).normalized() * simval.pistonBaseLength;
    simval.pistonBaseEnd = simval.pistonOrigin + simval.pistonDirection;

    auto piston = createPiston(simval);

    CV::color(1, 0, 0);
    for(auto &p : crankShaft){
        draw(p, cam);
    }

    CV::color(0,1,0);
    for(auto &p : piston){
        draw(p, cam);
    }

    drawAxis(cam);
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
