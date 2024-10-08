#ifndef __PRIMITIVES_H__
#define __PRIMITIVES_H__

#include <vector>

#include "../Math/Vector3.h"
#include "../Misc/Tuple.h"

/// @brief Estrutura basica para um objeto 3D ser renderizado em wireframe.
/// Possui funcoes para a criacao de primitivas.
struct Primitive {
public:

    struct Face {
    public:
        std::vector<int> vertices;
        std::vector<int> normals;
    };

    /// @brief Lista de vertices do objeto
    std::vector<Vector3> vertexList;
    /// @brief Lista com a normais do objeto
    std::vector<Vector3> normalList;
    std::vector<std::vector<int>> edgeList; // usado para wireframe
    /// @brief Lista de faces. Cada
    std::vector<Face> faceList; // usado para modo com preenchimento
    
    // TODO: arrumar isso
    Vector3 color;

    /// @brief Transforma qualquer face que nao tenha 3 vertices um varias faces de 3 vertices.
    void Triangulate();

    /// @brief Cria uma esfera com coordenadas polares de raio unitario e centrada na origem.
    /// @param horizontalResolution Quantidade de linhas verticais. Essas linhas se movem na horizontal
    /// @param verticalResolution Quantidade de linhas horizontais. Essas linhas se movem na vertical
    /// @return 
    static Primitive createSphere(int horizontalResolution, int verticalResolution);

    /// @brief Cria um cilindro com coordenadas polares de raio unitario e centrado na origem.
    /// Ele esta alinhado no eixo Z.
    /// @param resolution Quantidade de vertices na base do cilindro
    /// @param height Altura do cilindro
    /// @param radius Raio do cilindro
    static Primitive createCylinder(int resolution, float height, float radius);

    /// @brief Cria um cubo centrado na origem.
    /// @param size Tamanho do lado do cubo
    static Primitive createCube(float size);

    /// @brief Cria uma engrenagem de raio medio 1, espessura 0.5, centrada na origem
    /// e orientada no eixo Z.
    /// @param teeth O numero de dentes que a engrenagem vai ter
    /// @param teethHeight A altura maxima de um dente. Isso significa que o raio minimo eh 1-teethHeight
    static Primitive createGear(int teeth, float teethHeight);
};

#endif