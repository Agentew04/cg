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

#pragma region P1Q4

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

#pragma endregion

#pragma region curves2d

Vector2 ctrlPts[4] = {
    Vector2(0,0),
    Vector2(50, 100),
    Vector2(100, 0),
    Vector2(150, 100)
};

void renderBezierBlendingFunctions(){
    float scaleX = 100;
    float scaleY = 100;

    for(float t=0; t<1; t+=0.0001){
        float x = t*scaleX;
        float y = std::pow(1-t, 3)*scaleY;
        CV::color(0,0,0);
        CV::point(x, y);

        y = (3*t * std::pow(1-t,2)) * scaleY;
        CV::color(1,0,0);
        CV::point(x, y);

        y = (3*t*t * (1-t)) * scaleY;
        CV::color(0,1,0);
        CV::point(x, y);

        y = (t*t*t) * scaleY;
        CV::color(0,0,1);
        CV::point(x, y);
    }
}

void renderBSplineBlendingFunctions(){
    float scaleX = 100;
    float scaleY = 100;

    for(float t=0; t<1; t+=0.0001){
        float x = t*scaleX;
        float y = std::pow(1-t, 3) * scaleY/6;
        CV::color(0,0,0);
        CV::point(x, y);

        y = (3*t*t*t - 6*t*t + 4) * scaleY/6;
        CV::color(1,0,0);
        CV::point(x, y);

        y = (-3*t*t*t + 3*t*t + 3*t + 1) * scaleY/6;
        CV::color(0,1,0);
        CV::point(x, y);

        y = t*t*t * scaleY/6;
        CV::color(0,0,1);
        CV::point(x, y);
    }
}


void renderControlPoints(){
    for(int i=0; i<4; i++){
        CV::color(0,0,0);
        CV::circleFill(ctrlPts[i].x, ctrlPts[i].y, 5, 10);
        if(i!=4-1){
            CV::lineDashed(ctrlPts[i], ctrlPts[i+1], 5, 3);
        }
    }
}

Vector2 lerp(Vector2 a, Vector2 b, float t){
    return a*(1-t) + b*t;
}

void renderBezierLerp(){
    for(float t=0; t<1; t+=0.001){
        Vector2 p0 = lerp(ctrlPts[0], ctrlPts[1], t);
        Vector2 p1 = lerp(ctrlPts[1], ctrlPts[2], t);
        Vector2 p2 = lerp(ctrlPts[2], ctrlPts[3], t);

        Vector2 p01 = lerp(p0, p1, t);
        Vector2 p12 = lerp(p1, p2, t);

        Vector2 p = lerp(p01, p12, t);
        CV::color(0,0,0);
        CV::point(p.x, p.y);
    }
}

void renderBezier(){
    for(float t=0; t<50; t+=0.001){
        float p1 = std::pow(1-t, 3);
        float p2 = (3*t * std::pow(1-t,2));
        float p3 = (3*t*t * (1-t));
        float p4 = (t*t*t);

        Vector2 p = ctrlPts[0]*p1 + ctrlPts[1]*p2 + ctrlPts[2]*p3 + ctrlPts[3]*p4;
        CV::color(0,1,1);
        CV::point(p.x, p.y);
    }
}

void renderBSpline(){
    for(float t=0; t<1; t+=0.001){
        float p1 = std::pow(1-t, 3) * 1/6;
        float p2 = (3*t*t*t - 6*t*t + 4) * 1/6;
        float p3 = (-3*t*t*t + 3*t*t + 3*t + 1) * 1/6;
        float p4 = t*t*t * 1/6;

        Vector2 p = ctrlPts[0]*p1 + ctrlPts[1]*p2 + ctrlPts[2]*p3 + ctrlPts[3]*p4;
        CV::color(0,1,1);
        CV::point(p.x, p.y);
    }
}

#pragma endregion

#pragma region cube3d

Vector3 cubeCoordinates[] = {
    Vector3(-0.5f, -0.5f, -0.5f),
    Vector3(0.5f, -0.5f, -0.5f),
    Vector3(0.5f, 0.5f, -0.5f),
    Vector3(-0.5f, 0.5f, -0.5f),
    Vector3(-0.5f, -0.5f, 0.5f),
    Vector3(0.5f, -0.5f, 0.5f),
    Vector3(0.5f, 0.5f, 0.5f),
    Vector3(-0.5f, 0.5f, 0.5f)
};
Vector3 cubeNormals[] = {
    Vector3(0,0,-1),
    Vector3(0,0,1),
    Vector3(0,-1,0),
    Vector3(0,1,0),
    Vector3(-1,0,0),
    Vector3(1,0,0)
};

Vector3 cubeScale = Vector3(1,1,1);

float cubeAngle = 0.0f;
float d = 150.0f;

// TODO mover essas funcoes para Vector3.cpp?
Vector3 rotateY(Vector3 point, float angle){
    float x = point.x * std::cos(angle) - point.z * std::sin(angle);
    float z = point.x * std::sin(angle) + point.z * std::cos(angle);
    return Vector3(x, point.y, z);
}

Vector3 translateZ(Vector3 point, float z){
    return Vector3(point.x, point.y, point.z + z);
}

Vector3 scale(Vector3 point, Vector3 factor){
    return Vector3(point.x * factor.x, point.y * factor.y, point.z * factor.z);
}

Vector2 perspectiveProjection(Vector3 point, float distance){
    float x = point.x * distance / point.z;
    float y = point.y * distance / point.z;
    return Vector2(x, y);
}


void drawObjPerspective(Model3D* obj){
    Vector3 p;

    Vector2 *output = new Vector2[obj->vertices.size()];

    for(int i=0; i<obj->vertices.size(); i++){
        p = obj->vertices[i];

        p = scale(p, cubeScale);
        p = rotateY(p, cubeAngle);
        p = translateZ(p, 5);

        output[i] = perspectiveProjection(p, d);
    }

    glBegin(GL_TRIANGLES);
    for(int i=0; i<obj->faces.size(); i++){
        Vector3 cameraDir = Vector3(0,0,1);
        Vector3 n1 = obj->normals[obj->faceNormals[i][0]];
        Vector3 n2 = obj->normals[obj->faceNormals[i][1]];
        Vector3 n3 = obj->normals[obj->faceNormals[i][2]];
        n1 = rotateY(n1, cubeAngle);
        n2 = rotateY(n2, cubeAngle);
        n3 = rotateY(n3, cubeAngle);

        if(cameraDir.dot(n1) > 0 && cameraDir.dot(n2) > 0 && cameraDir.dot(n3) > 0) continue;


        for(int j=0; j<obj->faces[i].size(); j++){
            Vector3 normal =  obj->normals[obj->faceNormals[i][j]];
            normal = rotateY(normal, cubeAngle);
            Vector3 sun = Vector3(-1,-1,-1);
            sun = sun / sun.magnitude;
            float intensity = sun.dot(normal);
            if(intensity < 0) intensity = 0;
            glColor3f(intensity, intensity, intensity);
            glVertex3f(output[obj->faces[i][j]].x, output[obj->faces[i][j]].y, 1);
            //CV::line(output[obj->faces[i][j]], output[obj->faces[i][(j+1)%obj->faces[i].size()]]);
        }
    }
    glEnd();

    CV::color(0,0,0);
    CV::text(
        Vector2(50,50),
        "d="+std::to_string((int)d),
        30
    );
    CV::text(
        Vector2(50,80),
        "size="+std::to_string(cubeScale.x),
        30
    );

    cubeAngle += 0.01f;
}

void drawCubePerspective(){
    Vector3 p;

    Vector2 output[8];
    for(int i=0; i<8; i++){
        p = cubeCoordinates[i];

        p = scale(p, cubeScale);
        p = rotateY(p, cubeAngle);
        p = translateZ(p, 5);

        output[i] = perspectiveProjection(p, d);
    }

    // draw cube wireframe(12 lines)
    for(int i=0; i<4; i++){
        CV::line(output[i], output[(i+1)%4]);
        CV::line(output[i+4], output[((i+1)%4)+4]);
        CV::line(output[i], output[i+4]);
    }
    // glBegin(GL_QUADS);
    // for(int i=0; i<6; i++){
    //     Vector3 cameraDir = Vector3(0,0,1);
    //     Vector3 n1 = cubeNormals[i];
    //     n1 = rotateY(n1, cubeAngle);

    //     if(cameraDir.dot(n1) > 0) continue;


    //     for(int j=0; j<4; j++){
    //         Vector3 normal =  cubeNormals[i];
    //         normal = rotateY(normal, cubeAngle);
    //         Vector3 sun = Vector3(-1,-1,-1);
    //         sun = sun / sun.magnitude;
    //         float intensity = sun.dot(normal);
    //         if(intensity < 0) intensity = 0;
    //         glColor3f(intensity, intensity, intensity);
    //         glVertex3f(output[(i*4)+j].x, output[(i*4)+j].y, 1);
    //         //CV::line(output[obj->faces[i][j]], output[obj->faces[i][(j+1)%obj->faces[i].size()]]);
    //     }
    // }

    CV::color(0,0,0);
    CV::text(
        Vector2(50,50),
        "d="+std::to_string((int)d),
        30
    );
    CV::text(
        Vector2(50,80),
        "size="+std::to_string(cubeScale.x),
        30
    );

    cubeAngle += 0.01f;
}

#pragma endregion

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

    ctrlPts[2] = Vector2(x - screenWidth/2, y - screenHeight/2);

    float middleX = x - screenWidth/2;
    float middleY = y - screenHeight/2;
    d = middleY;
    cubeScale = Vector3(1 + middleX/100, 1 + middleX/100, 1 + middleX/100);

    if(state == 0){
    }else if(state == 1){
        // mouse up
    }
}



void render()
{
    CV::clear(1,1,1);
    CV::translate(screenWidth/2, screenHeight/2);
    CV::color(1,0,0);

    //drawCubePerspective();

    //drawObjPerspective(ObjLoader::get("monkey"));
    drawObjPerspective(ObjLoader::get("person"));

    Sleep(10); //nao eh controle de FPS. Somente um limitador de FPS.
}

int main(void)
{
    FontManager::load(".\\Playground\\assets\\fonts\\jetbrainsmono.font", FontName::JetBrainsMono);
    ObjLoader::load(".\\Playground\\assets\\models\\monkey-normal.obj", "monkey");
    ObjLoader::load(".\\Playground\\assets\\models\\person.obj", "person");
    CV::init(&screenWidth, &screenHeight, "Canvas2D - Custom Template", false, true);
    CV::run();
    cleanup();
}
