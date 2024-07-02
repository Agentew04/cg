// #include <GL/glut.h>
// #include <GL/freeglut_ext.h> //callback da wheel do mouse.

// #include <math.h>
// #include <iostream>
// #include <stdio.h>
// #include <stdlib.h>
// #include <sstream>
// #include <iomanip>

// #include "Fonts/FontManager.h"
// #include "UI/CursorManager.h"

// #include "3D/Buffer.h"

// #include "Gl.h"

// // largura e altura inicial da tela . Alteram com o redimensionamento de tela.
// int screenWidth = 1280, screenHeight = 720;
// Vector2 mousePos;

// GL gl;

// void update(float delta)
// {
// }

// void render()
// {
// }

// void cleanup()
// {
//     CursorManager::freeResources();
//     ObjLoader::free(); // libera os recursos do FontManager
// }

// // funcao chamada toda vez que uma tecla for pressionada.
// void keyboard(unsigned char key, int x, int y)
// {
// }

// // funcao chamada toda vez que uma tecla for liberada
// void keyboardUp(unsigned char key, int x, int y)
// {
// }

// // funcao para tratamento de mouse: cliques, movimentos e arrastos
// void mouseInteract(int button, int state, int wheel, int direction)
// {
//     CursorManager::startFrame();

//     if (state == 0)
//     {
//         // down
//     }
//     else if (state == 1)
//     {
//         // up
//     }
//     CursorManager::applyCursor();
// }

// void mouseMove(int x,int y){
//     CursorManager::startFrame();
//     mousePos = Vector2(x, y);
//     CursorManager::applyCursor();
// }

// void init()
// {
// }

// int main(void)
// {
//     FontManager::load("./Template/assets/fonts/jetbrainsmono.font", FontName::JetBrainsMono);

//     GLOptions opt;
//     opt.fullscreen = true;
//     opt.antiAliasing = true;
//     opt.vsync = true;
//     opt.defaultScreenSize = Vector2(1280,720);

//     GL::gl.init(opt);

//     GL::gl.start();

//     // glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
//     // glutInitWindowSize(screenWidth, screenHeight);
//     // // glutInitWindowPosition (450, 10);
//     // glutCreateWindow("Trabalho 5 - Rodrigo Appelt - \"Avião Florestal\"");

//     // // inicializa valores base
//     // glMatrixMode(GL_PROJECTION);
//     // glLoadIdentity();
//     // gluPerspective(20, 1.77, 0.1, 100);
//     // glMatrixMode(GL_MODELVIEW);
//     // glClearColor(0, 0, 0, 1);
//     // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
//     // glEnable(GL_DEPTH_TEST);

//     // // define callbacks
//     // glutDisplayFunc(render);
//     // glutIdleFunc(render);
//     // glutMotionFunc(mouseMove);
//     // glutMouseFunc(mouseInteract);
//     // glutKeyboardFunc(keyboard);

//     cleanup();
// }

#include <GL/glut.h>
#include <GL/freeglut_ext.h>

#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

#define SCREEN_X 1280
#define SCREEN_Y 720

int polygonMode = 1;

float abertura = 20.0;
float znear = 1;
float zfar = 23;
float aspect = 16.0/9.0;

////////////////////////////////////////////////////////////////////////////////////////
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
    gluLookAt(0, 0, 15, // from. Posicao onde a camera esta posicionada
              0, 0, 0,  // to. Posicao absoluta onde a camera esta vendo
              0, 1, 0); // up. Vetor Up.

    

    // todos os objetos estao definidos na origem do sistema global fixo
    // e sao transladados para a posicao destino.
    glColor3f(1, 1, 1);
    glBegin(GL_POLYGON);
    glVertex3f(-1, 0, 1);
    glVertex3f(1, 0, 1);
    glVertex3f(1, 0, -1);
    glVertex3f(-1, 0, -1);
    glEnd();

    static float angle = 0;
    // bule 1
    glPushMatrix();
    glColor3f(1, 0, 1);
    glTranslated(-0.5, 0.15, 0.5);
    glRotated(angle, 0, 1, 0);
    glutWireTeapot(0.2);
    glPopMatrix();

    // bule 2
    glPushMatrix();
    glColor3f(0, 1, 0);
    glTranslated(0.5, 0.15, -0.5);
    glRotated(90, 0, 1, 0);
    glutWireTeapot(0.2);
    glPopMatrix();

    angle += 1.0f;
    glutSwapBuffers();
}

// faz a leitura da entrada do usuario
void keyboard(unsigned char key, int x, int y)
{
    // printf("%c", key);
    key = tolower(key);
    switch (key)
    {
    case 27:
        exit(0);
        break;
    case 'f':
        glutFullScreenToggle();
        break;
    }
}

void MotionFunc(int x, int y)
{

}

void MouseFunc(int botao, int estado, int x, int y)
{

}

void initOpenGL(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(59, 16.0/9.0, 0.1, 100);
    glMatrixMode(GL_MODELVIEW);
    
    glClearColor(0.0, 0.0, 0.0, 0.0); // Set background color to black and opaque
    glClearDepth(1.0);                // Set background depth to farthest
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);           // Set the type of depth-test
    glShadeModel(GL_SMOOTH);          // Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);  // Enable light #0

    GLfloat light_ambient[] = { 0.2f, 0.2f, 0.2f, 1.0f };
    GLfloat light_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_position[] = { 1.0f, 1.0f, 1.0f, 0.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    // Set material properties
    GLfloat mat_ambient[] = { 0.7f, 0.7f, 0.7f, 1.0f };
    GLfloat mat_diffuse[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat mat_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat mat_shininess[] = { 50.0f };

    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
}

// Function to handle window resizing
void reshape(GLsizei width, GLsizei height) {
    if (height == 0) height = 1;  // To prevent divide by zero

    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    glViewport(0, 0, width, height);  // Set the viewport to cover the new window

    glMatrixMode(GL_PROJECTION);  // Set the aspect ratio of the clipping volume to match the viewport
    glLoadIdentity();             // Reset the projection matrix
    gluPerspective(59, aspect, 0.1f, 100.0f);  // Set up a perspective projection matrix

    glMatrixMode(GL_MODELVIEW);   // Return to the modelview matrix mode
    glLoadIdentity();             // Reset the modelview matrix
}

////////////////////////////////////////////////////////////////////////////////////////
int main()
{
    int argc = 0;
    // char *argv[1] = {"teste"};
    glutInit(&argc, NULL);

    glutSetOption(GLUT_MULTISAMPLE, 4);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE | GLUT_DEPTH);

    glutInitWindowSize(SCREEN_X, SCREEN_Y);
    glutCreateWindow("GluLookAt Demo");

    // init
    initOpenGL();


    glutDisplayFunc(display);
    glutMotionFunc(MotionFunc);
    glutMouseFunc(MouseFunc);
    glutIdleFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutFullScreen();

    glutMainLoop();
    return 0;
}
