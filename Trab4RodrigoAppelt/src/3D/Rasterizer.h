#ifndef __RASTERIZER_H__
#define __RASTERIZER_H__

#include <vector>
#include <functional>

#include "Primitive.h"
#include "Buffer.h"
#include "Camera.h"

class Rasterizer {
public:
    /// @brief Limpa buffers e renderiza primitivas no color buffer.
    /// @param primitives A lista de primitivas que vai ser renderizada
    /// @param camera A camera que vai ser usada para renderizar
    /// @param colorBuffer O buffer the cor
    /// @param depthBuffer O buffer de profundidade
    static void Rasterize(
        std::vector<Primitive>& primitives, 
        Camera3D& camera,
        Buffer* colorBuffer,
        Buffer* depthBuffer,
        Vector3 lightDirection,
        float ambientLightIntensity,
        float scale,
        bool displayNormals
    );
private:
    static void BoundBox(
        const std::vector<Vector3>& vertexList,
        const Primitive::Face& face,
        Vector2& p1,
        Vector2& p2);

    static bool isInside(
        const std::vector<Vector3>& vertexList,
        const Primitive::Face& face,
        Vector2 point
    );

    static float interpolateZ(
        const std::vector<Vector3>& vertexList,
        const Primitive::Face& face,
        Vector2 point
    );

    /// @brief Usa coordenadas baricentricas para interpolar a normal de um ponto em um poligono
    static Vector3 interpolateNormal(
        const std::vector<Vector3>& vertexList,
        const std::vector<Vector3>& normalList,
        const Primitive::Face& face,
        Vector2 point
    );
};

#endif