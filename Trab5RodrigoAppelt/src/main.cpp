/*

Computacao Grafica
Trabalho 5 - Floresta GL(Grande e Larga)

Aluno: Rodrigo Appelt

Requisitos implementados:
    - Movimento de camera suave + Controle de FPS
    - Uso de vetores e transformacoes
    - Camera com vetores(posicao e rotacao)
    - Todos modelos tem normais. Inclui iluminacao
    - Tetraedro Refinado na copa das arvores
    - Wireframe e iluminacao por vertice

Extras implementados:
    - Organizacao do codigo em estilo Ator/GameObject - Componente
    - Leitura de arquivos de modelos .obj 3D com materiais.
    - Mais detalhes no tronco das arvores
    - Inclusao de texturas
    - Skybox com imagem 360 graus
    - Terreno curvo com superficie Bezier

Como usar:
    1. Com as teclas W e S voce olha para cima e para baixo
    2. Com as teclas A e D voce olha para esquerda e direita
    3. Segurando a tecla ESPACO voce avanca para frente,
       na direcao que voce esta olhando
    4. Apertando G voce troca entre modo wireframe e preenchido

 */

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
float aspect = 16.0 / 9.0;

#include "Math/Vector3.h"
#include "Engine/Material.h"
#include "Keyboard.h"
#include "Manager.h"

Manager mngr;
float lastFrame = 0;
void display(void)
{
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   float time = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
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

void initOpenGL()
{
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(abertura, aspect, znear, zfar); // Set up a perspective projection matrix
   glMatrixMode(GL_MODELVIEW);

   glClearColor(0.0, 0.0, 0.0, 0.0); // Set background color to black and opaque
   glClearDepth(1.0);                // Set background depth to farthest
   glEnable(GL_DEPTH_TEST);
   glDepthFunc(GL_LEQUAL);                            // Set the type of depth-test
   glShadeModel(GL_SMOOTH);                           // Enable smooth shading
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST); // Nice perspective corrections

   glEnable(GL_TEXTURE);
   glEnable(GL_TEXTURE_2D);

   glEnable(GL_MAP2_TEXTURE_COORD_2);
   glEnable(GL_MAP2_VERTEX_3);
}

void reshape(GLsizei width, GLsizei height)
{
   if (height == 0)
      height = 1;

   GLfloat aspect = (GLfloat)width / (GLfloat)height;

   glViewport(0, 0, width, height);

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(abertura, aspect, znear, zfar);

   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

////////////////////////////////////////////////////////////////////////////////////////
int main()
{
   int argc = 0;
   glutInit(&argc, NULL);

   glutSetOption(GLUT_MULTISAMPLE, 4);
   glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE | GLUT_DEPTH);

   glutInitWindowSize(SCREEN_X, SCREEN_Y);
   glutCreateWindow("Trab5RodrigoAppelt - Floresta GL(Grande e Larga)");

   // init
   initOpenGL();

   ((BOOL(WINAPI*)(int))wglGetProcAddress("wglSwapIntervalEXT"))(1);

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
