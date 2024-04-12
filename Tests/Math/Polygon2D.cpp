#include "Polygon2D.h"

Polygon2D::Polygon2D() {
}

Polygon2D::Polygon2D(std::vector<Vector2> vertices) {
    this->vertices = vertices;
}

Polygon2D::Polygon2D(const Polygon2D &p) {
    this->vertices = p.vertices;
}

Polygon2D::~Polygon2D() {
    this->vertices.clear();
}

bool Polygon2D::pointInside(Vector2 point) {
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