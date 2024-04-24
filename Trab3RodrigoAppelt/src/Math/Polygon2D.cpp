#include "Polygon2D.h"

#include <map>
#include <chrono>

#include "../Misc/Tuple.h"


// forward declaration
class CV{
public:
    // returns the current time in seconds
    static float time();
};

std::map<Tuple<uint32_t,uint32_t>,float> lastCollisionTime;

Rectangle2D::Rectangle2D(Vector2 position, Vector2 size, bool sizeAsP2) {
    this->position = position;
    if (sizeAsP2) {
        float sizeX = std::fabs(size.x - position.x);
        float sizeY = std::fabs(size.y - position.y);
        this->size = Vector2(sizeX, sizeY);
    } else {
        this->size = size;
    }
}

Rectangle2D::Rectangle2D() : position(Vector2(0, 0)), size(Vector2(0, 0)) {}

Rectangle2D::Rectangle2D(const Rectangle2D &r) : position(r.position), size(r.size) {}

bool Rectangle2D::pointInside(const Vector2& point) const {
    return point.x >= position.x && point.x <= position.x + size.x && point.y >= position.y && point.y <= position.y + size.y;
}

Collision Rectangle2D::intersects(const Shape2D& shape) const {
    auto tuple = Tuple<uint32_t, uint32_t>::make(id, shape.id);
    if(id != 0 && shape.id != 0){
        if(lastCollisionTime.find(tuple) != lastCollisionTime.end()){
            float now = CV::time();
            if(now - lastCollisionTime[tuple] < collisionWait){
                return Collision{false, Vector2(0, 0)};
            }
        }
    }else{
        std::cout << "collider with id 0. Not caching" << std::endl;
    }

    if (dynamic_cast<const Rectangle2D*>(&shape) != nullptr) {
        Rectangle2D r = dynamic_cast<const Rectangle2D&>(shape);

        // rect rect collision
        if (position.x + size.x < r.position.x || position.x > r.position.x + r.size.x 
            || position.y + size.y < r.position.y || position.y > r.position.y + r.size.y) {
            return Collision{false, Vector2(0, 0)};
        }

        Vector2 normal = Vector2(0, 0);
        if (position.x < r.position.x) {
            normal = Vector2(-1, 0);
        } else if (position.x > r.position.x) {
            normal = Vector2(1, 0);
        } else if (position.y < r.position.y) {
            normal = Vector2(0, -1);
        } else if (position.y > r.position.y) {
            normal = Vector2(0, 1);
        }
        lastCollisionTime[tuple] = CV::time();
        return Collision{true, normal};
    }

    if (dynamic_cast<const Circle2D*>(&shape) != nullptr) {
        Circle2D c = dynamic_cast<const Circle2D&>(shape);

        // check circle rect collision
        Vector2 closestPoint = Vector2(std::max(position.x, std::min(c.position.x, position.x + size.x)),
                                       std::max(position.y, std::min(c.position.y, position.y + size.y)));
        if ((closestPoint - c.position).magnitude <= c.radius) {
            lastCollisionTime[tuple] = CV::time();
            return Collision{true, (closestPoint - c.position).normalized()};
        }
        return Collision{false, Vector2(0, 0)};
    }

    std::cout << "Could not get normal data from collision" << std::endl;
    return Collision{false, Vector2(0, 0)};
}

Circle2D::Circle2D(Vector2 position, float radius) : position(position), radius(radius) {}

Circle2D::Circle2D() : position(Vector2(0, 0)), radius(0) {}

Circle2D::Circle2D(const Circle2D &c): position(c.position), radius(c.radius){}

bool Circle2D::pointInside(const Vector2& point) const{
    return (point - position).magnitude <= radius;
}


Collision Circle2D::intersects(const Shape2D& shape) const{
    
    auto tuple = Tuple<uint32_t,uint32_t>::make(id, shape.id);
    if(id != 0 && shape.id != 0){
        float now = CV::time();
        if(lastCollisionTime.find(tuple) != lastCollisionTime.end()){
            if(now - lastCollisionTime[tuple] < collisionWait){
                return Collision{false, Vector2(0, 0)};
            }
        }
    }else{
        std::cout << "collider with id 0. Not caching" << std::endl;
    }

    // if shape is circle
    if (dynamic_cast<const Circle2D*>(&shape) != nullptr) {
        Circle2D c = dynamic_cast<const Circle2D&>(shape);
        // circle circle collision
        if((c.position - position).magnitude <= c.radius + radius){
            lastCollisionTime[tuple] = CV::time();
            return Collision{true, (c.position - position).normalized()};
        }
        return Collision{false, Vector2(0, 0)};
    }

    // if shape is a rect
    if (dynamic_cast<const Rectangle2D*>(&shape) != nullptr) {
        Rectangle2D r = dynamic_cast<const Rectangle2D&>(shape);
        Vector2 normal;

        // codigo de colisao circle-rect tirado de
        // https://www.jeffreythompson.org/collision-detection/circle-rect.php
        
        float cx = position.x;
        float cy = position.y;
        float rx = r.position.x;
        float ry = r.position.y;
        float rw = r.size.x;
        float rh = r.size.y;

        // temporary variables to set edges for testing
        float testX = cx;
        float testY = cy;

        // which edge is closest?
        if (cx < rx){
            testX = rx;      // test left edge
            normal = Vector2(-1, 0);
        }else if (cx > rx+rw){
            testX = rx+rw;   // right edge
            normal = Vector2(1, 0);
        }
        if (cy < ry) {
            testY = ry;      // top edge
            normal = Vector2(0, -1);
        }
        else if (cy > ry+rh) {
            testY = ry+rh;   // bottom edge
            normal = Vector2(0, 1);
        }

        // get distance from closest edges
        float distX = cx-testX;
        float distY = cy-testY;
        float distance = sqrt( (distX*distX) + (distY*distY) );

        // if the distance is less than the radius, collision!
        if (distance <= radius) {
            lastCollisionTime[tuple] = CV::time();
            return Collision{true, normal};
        }
        return Collision{false, Vector2(0, 0)};
    }

    // discrete circle
    int colliderResolution = 16;
    float stepSize = 2 * 3.1415 / colliderResolution;
    for (int i = 0; i < colliderResolution; i++) {
        float x = radius * cos(i * stepSize);
        float y = radius * sin(i * stepSize);
        if (shape.pointInside(position + Vector2(x, y))) {
            lastCollisionTime[tuple] = CV::time();
            #if PHYSICS_DEBUG
            std::cout << "Circle(" << id << ") Generic(" << shape.id << ") collision" << std::endl;
            #endif
            return Collision{true, (position - (position + Vector2(x, y))).normalized()};
        }
    }
    return Collision{false, Vector2(0, 0)};
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

Collision Polygon2D::intersects(const Shape2D& shape) const{
    // TODO consertar isso, por algum motivo n passa nos testes
    std::cout << "not implemented polygon collision" << std::endl;
    return Collision{false, Vector2(0,0)};
    // for(auto &vert:vertices){
    //     if(shape.pointInside(vert)){
    //         std::cout << "normal not available for polygon collision" << std::endl;
    //         return Collision{true, Vector2(0, 0)};
    //     }
    // }
    // return Collision{false, Vector2(0, 0)};
}
