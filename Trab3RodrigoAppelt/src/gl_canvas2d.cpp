/**
 *   Programa para ilustrar os elementos mais basicos do OpenGL e Glut.
 *   - Apresenta os principais recursos do sistema de Janelas GLUT
 *
 *   Autor: Cesar Tadeu Pozzer
 *   UFSM - 2020
 *
 *   pozzer@inf.ufsm.br
 *   pozzer3@gmail.com
 *
 * Referencias GLUT: http://www.opengl.org/documentation/specs/glut/
 *                   http://www.opengl.org/documentation/specs/glut/spec3/node1.html
 **/

#include "gl_canvas2d.h"
#include <GL/glut.h>

int *scrWidth, *scrHeight; // guarda referencia para as variaveis de altura e largura da main()

// conjunto de cores predefinidas. Pode-se adicionar mais cores.
float Colors[14][3] =
    {
        {0, 0, 0},       // Black
        {0.5, 0.5, 0.5}, // Gray
        {1, 0, 0},       // Red
        {0, 1, 0},       // Green
        {0, 0, 1},       // Blue
        {0, 1, 1},       // Cyan
        {1, 0, 1},       // Magenta
        {1, 1, 0},       // Yellow
        {1, 0.5, 0},     // Orange
        {0.5, 0, 0},     // Brown
        {0.5, 0.5, 0},   // Olive
        {0, 0.5, 0.5},   //
        {0.5, 0, 0.5},   //
        {1, 1, 1},       // white
};

void ConvertMouseCoord(int button, int state, int wheel, int direction, int x, int y);

// funcoes de CALLBACK da biblioteca Glut
void keyboard(int key);
void keyboardUp(int key);
void specialUp(int key);
void mouse(int bt, int st, int wheel, int direction, int x, int y);
void mouseWheelCB(int wheel, int direction, int x, int y);
void render();
void update(float delta);

void CV::getSize(int *w, int *h)
{
    *w = *scrWidth;
    *h = *scrHeight;
}

void CV::point(float x, float y)
{
    glBegin(GL_POINTS);
    glVertex2d(x, y);
    glEnd();
}

void CV::point(Vector2 p)
{
    glBegin(GL_POINTS);
    glVertex2d(p.x, p.y);
    glEnd();
}

void CV::line(float x1, float y1, float x2, float y2)
{
    glBegin(GL_LINES);
    glVertex2d(x1, y1);
    glVertex2d(x2, y2);
    glEnd();
}

void CV::line(float x1, float y1, float x2, float y2, float width)
{
    glLineWidth((GLfloat)width);
    line(x1, y1, x2, y2);
    glLineWidth(1.0);
}

void CV::line(Vector2 p1, Vector2 p2)
{
    line(p1.x, p1.y, p2.x, p2.y);
}

void CV::line(Vector2 p1, Vector2 p2, float width)
{
    line(p1.x, p1.y, p2.x, p2.y, width);
}

void CV::rect(float x1, float y1, float x2, float y2)
{
    glBegin(GL_LINE_LOOP);
    glVertex2d(x1, y1);
    glVertex2d(x1, y2);
    glVertex2d(x2, y2);
    glVertex2d(x2, y1);
    glEnd();
}

void CV::rect(Vector2 p1, Vector2 p2)
{
    CV::rect(p1.x, p1.y, p2.x, p2.y);
}

void CV::rectFill(float x1, float y1, float x2, float y2)
{
    glBegin(GL_QUADS);
    glVertex2d(x1, y1);
    glVertex2d(x1, y2);
    glVertex2d(x2, y2);
    glVertex2d(x2, y1);
    glEnd();
}
void CV::rectFill(Vector2 p1, Vector2 p2)
{
    glBegin(GL_QUADS);
    glVertex2d(p1.x, p1.y);
    glVertex2d(p1.x, p2.y);
    glVertex2d(p2.x, p2.y);
    glVertex2d(p2.x, p1.y);
    glEnd();
}

void CV::polygon(float vx[], float vy[], int elems)
{
    int cont;
    glBegin(GL_LINE_LOOP);
    for (cont = 0; cont < elems; cont++)
    {
        glVertex2d(vx[cont], vy[cont]);
    }
    glEnd();
}

void CV::polygonFill(float vx[], float vy[], int elems)
{
    int cont;
    glBegin(GL_POLYGON);
    for (cont = 0; cont < elems; cont++)
    {
        glVertex2d(vx[cont], vy[cont]);
    }
    glEnd();
}

void CV::polygonFill(Vector2 pos, Polygon2D *polygon)
{
    glBegin(GL_POLYGON);
    for (size_t cont = 0; cont < polygon->vertices.size(); cont++)
    {
        glVertex2d(
            pos.x + polygon->vertices[cont].x,
            pos.y + polygon->vertices[cont].y);
    }
    glEnd();
}

void CV::polygon(Vector2 pos, Polygon2D *polygon)
{
    glBegin(GL_LINE_LOOP);
    for (size_t cont = 0; cont < polygon->vertices.size(); cont++)
    {
        glVertex2d(
            pos.x + polygon->vertices[cont].x,
            pos.y + polygon->vertices[cont].y);
    }
    glEnd();
}

// existem outras fontes de texto que podem ser usadas
//   GLUT_BITMAP_9_BY_15
//   GLUT_BITMAP_TIMES_ROMAN_10
//   etc. Para mais detalhes, acesse https://www.opengl.org/resources/libraries/glut/spec3/node76.html
// Vejam tambem a funcao glutStrokeWidth(GLUTstrokeFont font, int character)
// Para textos de qualidade, ver:
//   https://www.freetype.org/
//   http://ftgl.sourceforge.net/docs/html/ftgl-tutorial.html
void CV::text(float x, float y, const char *t)
{
    void *font = GLUT_BITMAP_8_BY_13;
    CV::text(x, y, t, font);
}

float CV::textWidth(const char *t, void *font)
{
    int tam = (int)strlen(t);
    float width = 0;
    for (int c = 0; c < tam; c++)
    {
        width += glutBitmapWidth(font, t[c]);
    }
    return width;
}

float CV::fontHeight(void *font)
{
    return glutBitmapHeight(font);
}

void CV::text(float x, float y, const char *t, void *font)
{
    int tam = (int)strlen(t);
    int x0 = x;
    for (int c = 0; c < tam; c++)
    {
        if (t[c] == '\n')
        {
            x = x0;
            y += glutBitmapHeight(font);
            continue;
        }

        float charWidth = glutBitmapWidth(font, t[c]);
        glRasterPos2i(x, y);
        glutBitmapCharacter(font, t[c]);
        x += charWidth;
    }
}

void CV::text(Vector2 pos, const char *t)
{
    CV::text(pos.x, pos.y, t);
}

void CV::text(Vector2 pos, int valor)
{
    char str[10];
    snprintf(str, 10, "%d", valor);
    CV::text(pos.x, pos.y, str);
}

void CV::text(Vector2 pos, float valor)
{
    char str[10];
    snprintf(str, 10, "%.2f", valor);
    CV::text(pos.x, pos.y, str);
}

void CV::text(float x, float y, const char *t, void *font, TextAlign align)
{
    float width = textWidth(t, font);

    float finalX = x;
    if (align == TextAlign::RIGHT)
    {
        finalX = x - width;
    }
    else if (align == TextAlign::CENTER)
    {
        finalX = x - (width / 2);
    }
    CV::text(finalX, y, t, font);
}

void CV::text(float x, float y, const char *t, TextAlign align)
{
    CV::text(x, y, t, GLUT_BITMAP_8_BY_13, align);
}

void CV::clear(float r, float g, float b)
{
    glClearColor(r, g, b, 1);
}

void CV::clear(Vector3 rgb)
{
    glClearColor(rgb.x, rgb.y, rgb.z, 1);
}

void CV::obj(ObjFile *obj, Vector2 pos, Vector2 scale){
    glBegin(GL_TRIANGLES);
    for(auto face : obj->faces){
        for(auto vertex : face){
            auto v = obj->vertices[vertex-1];
            v = v.multiply(Vector3(scale.x, scale.y, 1));
            v = v + Vector3(pos.x, pos.y, 0);
            glVertex3f(v.x, v.y, v.z);
        }
    }
    glEnd();
}

void CV::circle(float x, float y, float radius, int div)
{
    float ang = 0, x1, y1;
    float inc = PI_2 / div;
    glBegin(GL_LINE_LOOP);
    for (int lado = 1; lado <= div; lado++) // GL_LINE_LOOP desenha um poligono fechado. Liga automaticamente o primeiro e ultimio vertices.
    {
        x1 = (cos(ang) * radius);
        y1 = (sin(ang) * radius);
        glVertex2d(x1 + x, y1 + y);
        ang += inc;
    }
    glEnd();
}

void CV::circle(Vector2 pos, float radius, int div)
{
    CV::circle(pos.x, pos.y, radius, div);
}

void CV::circleFill(float x, float y, float radius, int div)
{
    float ang = 0, x1, y1;
    float inc = PI_2 / div;
    glBegin(GL_POLYGON);
    for (int lado = 1; lado <= div; lado++) // GL_POLYGON desenha um poligono CONVEXO preenchido.
    {
        x1 = (cos(ang) * radius);
        y1 = (sin(ang) * radius);
        glVertex2d(x1 + x, y1 + y);
        ang += inc;
    }
    glEnd();
}

void CV::circleFill(Vector2 pos, float radius, int div)
{
    CV::circleFill(pos.x, pos.y, radius, div);
}

void CV::triangleFan(float vx[], float vy[], int n_elems)
{
    glBegin(GL_TRIANGLE_FAN);
    for (int i = 0; i < n_elems; i++)
    {
        glVertex2f(vx[i], vy[i]);
    }
    glEnd();
}

void CV::triangleFan(std::vector<Vector2> vertices, uint32_t count)
{
    int min = count;
    if (vertices.size() < count)
    {
        min = vertices.size();
    }
    if(min < 3){
        return;
    }
    glBegin(GL_TRIANGLE_FAN);
    for (size_t i = 0; i < min; i++)
    {
        glVertex2f(vertices[i].x, vertices[i].y);
    }
    glEnd();
}

// coordenada de offset para desenho de objetos.
// nao armazena translacoes cumulativas.
void CV::translate(float offsetX, float offsetY)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(offsetX, offsetY, 0);
}

void CV::translate(Vector2 offset)
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslated(offset.x, offset.y, 0);
}

void CV::color(float r, float g, float b)
{
    glColor3f(r, g, b);
}

void CV::color(int idx)
{
    glColor3fv(Colors[idx]);
}

void CV::color(float r, float g, float b, float alpha)
{
    glColor4f(r, g, b, alpha);
    // glTexImage2D(GL_TEXTURE_2D, 0, )
}

void CV::color(Vector3 rgb)
{
    glColor3f(rgb.x, rgb.y, rgb.z);
}

void CV::color(Vector3 rgb, float a)
{
    glColor4f(rgb.x, rgb.y, rgb.z, a);
}

void special(int key, int, int)
{
    keyboard(key + 100);
}

void specialUp(int key, int, int)
{
    keyboardUp(key + 100);
}

void keyb(unsigned char key, int, int)
{
    keyboard(key);
}

void keybUp(unsigned char key, int, int)
{
    keyboardUp(key);
}

void mouseClick(int button, int state, int x, int y)
{
    ConvertMouseCoord(button, state, -2, -2, x, y);
}

void mouseWheelCB(int wheel, int direction, int x, int y)
{
    ConvertMouseCoord(-2, -2, wheel, direction, x, y);
}

void motion(int x, int y)
{
    ConvertMouseCoord(-2, -2, -2, -2, x, y);
}

void ConvertMouseCoord(int button, int state, int wheel, int direction, int x, int y)
{
#if Y_CANVAS_CRESCE_PARA_CIMA == TRUE
    y = *scrHeight - y; // deve-se inverter a coordenada y do mouse se o y da canvas crescer para cima. O y do mouse sempre cresce para baixo.
#else
    // nao faz nada.
#endif
    mouse(button, state, wheel, direction, x, y);
}

// funcao chamada sempre que a tela for redimensionada.
void reshape(int w, int h)
{
    *scrHeight = h; // atualiza as variaveis da main() com a nova dimensao da tela.
    *scrWidth = w;

    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // cria uma projecao ortografica com z entre (-1, 1).
#if Y_CANVAS_CRESCE_PARA_CIMA == TRUE
    // parametros: left, right, bottom, top
    gluOrtho2D(0.0, w, 0.0, h); // o eixo y cresce para cima.
#else
    // parametros: left, right, bottom, top
    gluOrtho2D(0.0, w, h, 0.0); // o eixo y cresce para baixo
#endif

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

// definicao de valores para limpar buffers
void inicializa()
{
    glClearColor(1, 1, 1, 1);
    glPolygonMode(GL_FRONT, GL_FILL);
}

static int lastTime = 0;
// buffer of deltas to calculate FPS
#define FPS_BUFFER_SIZE 60
static float deltas[FPS_BUFFER_SIZE] = {};
float deltaTime;

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    int time = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = ((float)time - lastTime) / 1000.0f;
    lastTime = time;

    // shift all elements to the left
    for (int i = 0; i < FPS_BUFFER_SIZE - 1; i++)
    {
        deltas[i] = deltas[i + 1];
    }
    deltas[FPS_BUFFER_SIZE - 1] = deltaTime;

    update(deltaTime);
    render();

    glFlush();
    glutSwapBuffers();
}

float CV::delta(){
    return deltaTime;
}

float CV::time(){
    return glutGet(GLUT_ELAPSED_TIME) / 1000.0f;
}

float CV::fps()
{
    // calcular media dos deltas no vetor
    float sum = 0;
    for (int i = 0; i < FPS_BUFFER_SIZE; i++)
    {
        sum += deltas[i];
    }
    return ((float)FPS_BUFFER_SIZE) / sum;
}

////////////////////////////////////////////////////////////////////////////////////////
//  inicializa o OpenGL
////////////////////////////////////////////////////////////////////////////////////////
void CV::init(int *w, int *h, const char *title, bool antiAliasing)
{
    int argc = 0;
    glutInit(&argc, NULL);

    scrHeight = h;
    scrWidth = w;

    // habilita MSAA
    if (antiAliasing)
    {
        glutSetOption(GLUT_MULTISAMPLE, 8);
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE);
    }
    else
    {
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    }
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);

    glutInitWindowSize(*w, *h);
    glutInitWindowPosition(50, 50);
    glutCreateWindow(title);

    inicializa();

    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyb);
    glutKeyboardUpFunc(keybUp);
    glutSpecialUpFunc(specialUp);
    glutSpecialFunc(special);

    glutIdleFunc(display);
    glutMouseFunc(mouseClick);
    glutPassiveMotionFunc(motion);
    glutMotionFunc(motion);
    glutMouseWheelFunc(mouseWheelCB);

    printf("GL Version: %s\n", glGetString(GL_VERSION));
}

void CV::run()
{
    glutMainLoop();
}

void CV::close()
{
    glutLeaveMainLoop();
}
