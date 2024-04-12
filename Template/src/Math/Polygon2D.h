#ifndef __POLYGON_H__
#define __POLYGON_H__

#include <vector>
#include "Vector2.h"

class Polygon2D {
public:

    /// @brief Cria um poligono vazio sem nenhum ponto
    Polygon2D();
    /// @brief Cria um poligono
    /// @param vertices Os vertices do poligono
    Polygon2D(std::vector<Vector2> vertices);
    /// @brief Clona um poligono
    Polygon2D(const Polygon2D &p);
    /// @brief Destrutor
    ~Polygon2D();

    /// @brief Checa se um ponto se encontra dentro do poligono convexo
    bool pointInside(Vector2 point);

    std::vector<Vector2> vertices;
private:

    
    // inline int orientation(Vector2 p, Vector2 q, Vector2 r){
    //     int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
    //     if (val == 0) return 0;
    //     return (val > 0)? 1: 2;
    // }
};

#endif