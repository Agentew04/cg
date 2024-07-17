#ifndef __CANVAS_2D__H__
#define __CANVAS_2D__H__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include <GL/glut.h>
#include <GL/freeglut_ext.h> //callback da wheel do mouse.

#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Polygon2D.h"
#include "UI/UIDefinitions.h"
#include "3D/ObjLoader.h"
#include "Fonts/FontManager.h"

#define PI_2 6.28318530717
#define PI   3.1415926535897932384626433832795

#define Y_CANVAS_CRESCE_PARA_CIMA 0


/// @brief Classe estatica que define as funcoes
/// disponibilizadas pela Canvas2D.
/// @remarks Se bem que agora ja esta uma Canvas2.5D
class CV
{
public:

    /// @brief Retorna a largura e altura da janela.
    static void getSize(int* w, int* h);

    //funcoes para desenho de ponto e linha
    static void point(float x, float y); //coordeandas do ponto
    static void point(Vector2 pos); //coordeandas do ponto

    static void line( float x1, float y1, float x2, float y2 ); //coordenadas da linha x1, y1, x2, y2
    static void line( Vector2 p1, Vector2 p2 ); //coordenadas da linha (p1, p2)
    static void line( Vector2 p1, Vector2 p2, float width); //coordenadas da linha (p1, p2)
    static void line( float x1, float y1, float x2, float y2, float width);
    static void lineDashed(Vector2 p1, Vector2 p2, float segmentLength, float width);

    //desenha um retangulo alinhado nos eixos x e y
    static void rect( float x1, float y1, float x2, float y2 ); //coordenadas do retangulo x1, y1, x2, y2
    static void rect( Vector2 p1, Vector2 p2 ); //coordenadas do retangulo (p1, p2) - TO DO

    static void rectFill( float x1, float y1, float x2, float y2 ); //coordenadas do retangulo x1, y1, x2, y2
    static void rectFill( Vector2 p1, Vector2 p2 ); //coordenadas do retangulo (p1, p2)

    //desenha um poligono CONVEXO. Para um retangulo, deve-se passar 4 vertices
    static void polygon(float vx[], float vy[], int n_elems);
    static void polygonFill(float vx[], float vy[], int n_elems);
    static void polygonFill(Vector2 pos, Polygon2D *poly);
    static void polygon(Vector2 pos, Polygon2D *poly);

    //centro e raio do circulo
    static void circle( float x, float y, float radius, int div );
    static void circle( Vector2 pos, float radius, int div );

    static void circleFill( float x, float y, float radius, int div );
    static void circleFill( Vector2 pos, float radius, int div );

    static void triangleFan(float vx[], float vy[], int n_elems);
    static void triangleFan(std::vector<Vector2> vertices, uint32_t count);


    //especifica a cor de desenho e de limpeza de tela
    static void color(float r, float g, float b);
    static void color(Vector3 rgb);
    static void color(Vector3 rgb, float a);
    static void color(float r, float g, float b, float alpha);

    static void clear(float r, float g, float b);
    static void clear(Vector3 rgb);

    //3d
    static void obj(Model3D* obj, Vector2 pos, Vector2 scale);

    //desenha texto na coordenada (x,y)
    static float textWidth(const char *t, void* font);
    static float fontHeight(void* font);
    static void text(float x, float y, const char *t);
    static void text(float x, float y, const char *t, TextAlign align);
    static void text(float x, float y, const char *t, void* font);
    static void text(float x, float y, const char *t, void* font, TextAlign align);
    static void text(Vector2 pos, const char *t);  //varias funcoes ainda nao tem implementacao. Faca como exercicio
    static void text(Vector2 pos, int valor);      //varias funcoes ainda nao tem implementacao. Faca como exercicio
    static void text(Vector2 pos, float valor);    //varias funcoes ainda nao tem implementacao. Faca como exercicio
    static void text(Vector2 pos, const std::string& text,
        float pt = 25.0f,
        FontName font = FontName::JetBrainsMono,
        UIPlacement placement = UIPlacement::CENTER);

    //coordenada de offset para desenho de objetos.
    static void translate(float x, float y);

    /// @brief Define uma nova origem no OpenGL para desenhos. Chamadas
    /// nao sao cumulativas.
    /// @param pos A nova origem.
    static void translate(Vector2 pos);

    /// @brief Retorna o tempo desde o inicio da aplicacao.
    static float time();

    /// @brief Retorna o tempo entre o frame atual e o frame anterior.
    /// @returns O tempo em segundos.
    static float delta();

    /// @brief Retorna uma media do FPS
    static float fps();

    /// @brief Define se o OpenGL deve desenhar apenas as linhas ou o preenchimento
    /// dos objetos.
    static void setWireframe(bool value);

    //funcao de inicializacao da Canvas2D. Recebe a largura, altura, e um titulo para a janela
    /// @brief Configura os parametros para a Canvas2D.
    /// @param w Largura da janela.
    /// @param h Altura da janela.
    /// @param title Titulo da janela.
    /// @param antiAliasing Se deve usar anti-aliasing. Padrao eh MSAA 4x.
    /// @param vsync Se deve usar vsync.
    static void init(int *w, int *h, const char *title, bool antiAliasing, bool vsync);

    /// @brief Sinaliza que o loop deve acabar. A execucao continua
    /// apos a primeira chamada de run().
    static void close();

    /// @brief Inicia o loop da aplicacao.
    static void run();
};

#endif
