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

float abertura = 59.0;
float znear = 0.1;
float zfar = 5000;
float aspect = 16.0/9.0;

#include "Math/Vector3.h"
#include "Engine/Material.h"
#include "Keyboard.h"
#include "Manager.h"

Manager mngr;
float lastFrame = 0;
void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    float time = glutGet(GLUT_ELAPSED_TIME)/1000.0;
    float delta = time - lastFrame;
    lastFrame = time;
    mngr.update(delta);
    mngr.render();
}

// faz a leitura da entrada do usuario
void keyboard(unsigned char key, int, int)
{
    // printf("%c", key);
    key = tolower(key);
    switch (key)
    {
    case ESC:
        exit(0);
        break;
    case 'f':
        glutFullScreenToggle();
        break;
    default:
        mngr.keyboard(key);
        break;
    }
}

void keyboardUp(unsigned char key, int, int)
{
    key = tolower(key);
    mngr.keyboardUp(key);
}

void initOpenGL(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(abertura, aspect, znear, zfar);  // Set up a perspective projection matrix
    glMatrixMode(GL_MODELVIEW);

    glClearColor(0.0, 0.0, 0.0, 0.0); // Set background color to black and opaque
    glClearDepth(1.0);                // Set background depth to farthest
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);           // Set the type of depth-test
    glShadeModel(GL_SMOOTH);          // Enable smooth shading
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections



    glEnable( GL_TEXTURE );
    glEnable( GL_TEXTURE_2D );

    glEnable(GL_MAP2_TEXTURE_COORD_2);
    glEnable(GL_MAP2_VERTEX_3);

}

// Function to handle window resizing
void reshape(GLsizei width, GLsizei height) {
    if (height == 0) height = 1;  // To prevent divide by zero

    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    glViewport(0, 0, width, height);  // Set the viewport to cover the new window

    glMatrixMode(GL_PROJECTION);  // Set the aspect ratio of the clipping volume to match the viewport
    glLoadIdentity();             // Reset the projection matrix
    gluPerspective(abertura, aspect, znear, zfar);  // Set up a perspective projection matrix

    glMatrixMode(GL_MODELVIEW);   // Return to the modelview matrix mode
    glLoadIdentity();             // Reset the modelview matrix
}

////////////////////////////////////////////////////////////////////////////////////////
int main()
{
    int argc = 0;
    glutInit(&argc, NULL);

    glutSetOption(GLUT_MULTISAMPLE, 4);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE | GLUT_DEPTH);

    glutInitWindowSize(SCREEN_X, SCREEN_Y);
    glutCreateWindow("GluLookAt Demo");

    // init
    initOpenGL();

    glutDisplayFunc(display);
    glutIdleFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutKeyboardUpFunc(keyboardUp);

    glutFullScreen();

    mngr.startloop();
    glutMainLoop();
    return 0;
}
