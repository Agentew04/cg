#include "Polygon2D.h"

Rectangle2D::Rectangle2D(Vector2 position, Vector2 size) : position(position), size(size) {}

Rectangle2D::Rectangle2D() : position(Vector2(0, 0)), size(Vector2(0, 0)) {}

Rectangle2D::Rectangle2D(const Rectangle2D &r) : position(r.position), size(r.size) {}

bool Rectangle2D::pointInside(const Vector2& point) const {
    return point.x >= position.x && point.x <= position.x + size.x && point.y >= position.y && point.y <= position.y + size.y;
}

bool Rectangle2D::intersects(const Shape2D& shape) const {
    return shape.pointInside(position) || shape.pointInside(position + Vector2(size.x, 0)) ||
           shape.pointInside(position + size) || shape.pointInside(position + Vector2(0, size.y));
}

Circle2D::Circle2D(Vector2 position, float radius) : position(position), radius(radius) {}

Circle2D::Circle2D() : position(Vector2(0, 0)), radius(0) {}

Circle2D::Circle2D(const Circle2D &c): position(c.position), radius(c.radius){}

bool Circle2D::pointInside(const Vector2& point) const{
    return (point - position).magnitude <= radius;
}

bool Circle2D::intersects(const Shape2D& shape) const{
    int colliderResolution = 16;
    float stepSize = 2 * 3.1415 / colliderResolution;
    for (int i = 0; i < colliderResolution; i++) {
        float x = radius * cos(i * stepSize);
        float y = radius * sin(i * stepSize);
        if (shape.pointInside(position + Vector2(x, y))) {
            return true;
        }
    }
    return false;
}


Polygon2D::Polygon2D() {}

Polygon2D::Polygon2D(std::vector<Vector2> vertices) : vertices(vertices) {}

Polygon2D::Polygon2D(const Polygon2D &p) : vertices(p.vertices) {}

Polygon2D::~Polygon2D() {
    this->vertices.clear();
}

bool Polygon2D::pointInside(const Vector2& point) const{
    // Check for empty polygon
    unsigned int i, j;
    bool c = false;
    for (i = 0, j = vertices.size()-1; i < vertices.size(); j = i++) {
        if ( ((vertices[i].y>point.y) != (vertices[j].y>point.y)) &&
        (point.x < (vertices[j].x-vertices[i].x) * (point.y-vertices[i].y) / (vertices[j].y-vertices[i].y) + vertices[i].x) )
        c = !c;
    }
    return c;
}

bool Polygon2D::intersects(const Shape2D& shape) const{
    for (unsigned int i = 0; i < vertices.size(); i++) {
        if (shape.pointInside(vertices[i])) {
            return true;
        }
    }
    return false;
}
