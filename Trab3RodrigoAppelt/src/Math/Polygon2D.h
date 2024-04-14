#ifndef __POLYGON_H__
#define __POLYGON_H__

#include <vector>
#include "Vector2.h"

class Shape2D{
public:
    virtual bool pointInside(const Vector2& point) const = 0;
    virtual bool intersects(const Shape2D& shape) const = 0;
};

class Rectangle2D : public Shape2D{
public:
    Rectangle2D(Vector2 position, Vector2 size);
    Rectangle2D();
    Rectangle2D(const Rectangle2D& r);
    bool pointInside(const Vector2& point) const;
    bool intersects(const Shape2D& shape) const;
    Vector2 position;
    Vector2 size;
};

class Circle2D : public Shape2D{
public:
    Circle2D(Vector2 position, float radius);
    Circle2D();
    Circle2D(const Circle2D& c);
    bool pointInside(const Vector2& point) const;
    bool intersects(const Shape2D& shape) const;
    Vector2 position;
    float radius;
};

class Polygon2D : public Shape2D{
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
    bool pointInside(const Vector2& point) const;

    /// @brief Checa se o poligono se intersecta com outro poligono
    bool intersects(const Shape2D& shape) const;

    std::vector<Vector2> vertices;
private:
    
};

#endif