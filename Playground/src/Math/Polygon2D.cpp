#include "Polygon2D.h"

#include <map>
#include <chrono>
#include <algorithm>

#include "../Misc/Tuple.h"

// forward declaration
class CV
{
public:
    // returns the current time in seconds
    static float time();
};

template <typename T>
T clamp(T value, T min, T max)
{
    return std::max(min, std::min(value, max));
}
std::map<Tuple<uint32_t, uint32_t>, float> lastCollisionTime;

Rectangle2D::Rectangle2D(Vector2 position, Vector2 size, bool sizeAsP2)
{
    this->position = position;
    if (sizeAsP2)
    {
        float sizeX = std::fabs(size.x - position.x);
        float sizeY = std::fabs(size.y - position.y);
        this->size = Vector2(sizeX, sizeY);
    }
    else
    {
        this->size = size;
    }
}

Rectangle2D::Rectangle2D() : position(Vector2(0, 0)), size(Vector2(0, 0)) {}

Rectangle2D::Rectangle2D(const Rectangle2D &r) : position(r.position), size(r.size) {}

bool Rectangle2D::pointInside(const Vector2 &point) const
{
    return point.x >= position.x && point.x <= position.x + size.x && point.y >= position.y && point.y <= position.y + size.y;
}

Collision Rectangle2D::intersects(const Shape2D &shape, bool ignoreCache) const
{
    auto tuple = Tuple<uint32_t, uint32_t>::make(id, shape.id);
    if (!ignoreCache)
    {
        if (id != 0 && shape.id != 0)
        {
            if (lastCollisionTime.find(tuple) != lastCollisionTime.end())
            {
                float now = CV::time();
                if (now - lastCollisionTime[tuple] < collisionWait)
                {
                    return Collision{false, Vector2(0, 0)};
                }
            }
        }
        else
        {
            std::cout << "collider with id 0. Not caching" << std::endl;
        }
    }

    if (dynamic_cast<const Rectangle2D *>(&shape) != nullptr)
    {
        Rectangle2D r = dynamic_cast<const Rectangle2D &>(shape);

        // rect rect collision
        if (position.x + size.x < r.position.x || position.x > r.position.x + r.size.x || position.y + size.y < r.position.y || position.y > r.position.y + r.size.y)
        {
            return Collision{false, Vector2(0, 0)};
        }

        Vector2 normal = Vector2(0, 0);
        if (position.x < r.position.x)
        {
            normal = Vector2(-1, 0);
        }
        else if (position.x > r.position.x)
        {
            normal = Vector2(1, 0);
        }
        else if (position.y < r.position.y)
        {
            normal = Vector2(0, -1);
        }
        else if (position.y > r.position.y)
        {
            normal = Vector2(0, 1);
        }
        lastCollisionTime[tuple] = CV::time();
        return Collision{true, normal};
    }

    if (dynamic_cast<const Circle2D *>(&shape) != nullptr)
    {
        Circle2D c = dynamic_cast<const Circle2D &>(shape);

        // check circle rect collision
        Vector2 closestPoint = Vector2(std::max(position.x, std::min(c.position.x, position.x + size.x)),
                                       std::max(position.y, std::min(c.position.y, position.y + size.y)));
        if ((closestPoint - c.position).magnitude <= c.radius)
        {
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

Circle2D::Circle2D(const Circle2D &c) : position(c.position), radius(c.radius) {}

bool Circle2D::pointInside(const Vector2 &point) const
{
    return (point - position).magnitude <= radius;
}

Collision Circle2D::intersects(const Shape2D &shape, bool ignoreCache) const
{

    auto tuple = Tuple<uint32_t, uint32_t>::make(id, shape.id);
    if (!ignoreCache)
    {
        if (id != 0 && shape.id != 0)
        {
            float now = CV::time();
            if (lastCollisionTime.find(tuple) != lastCollisionTime.end())
            {
                if (now - lastCollisionTime[tuple] < collisionWait)
                {
                    return Collision{false, Vector2(0, 0)};
                }
            }
        }
        else
        {
            std::cout << "collider with id 0. Not caching" << std::endl;
        }
    }

    // if shape is circle
    if (dynamic_cast<const Circle2D *>(&shape) != nullptr)
    {
        Circle2D c = dynamic_cast<const Circle2D &>(shape);
        // circle circle collision
        if ((c.position - position).magnitude <= c.radius + radius)
        {
            lastCollisionTime[tuple] = CV::time();
            return Collision{true, (c.position - position).normalized()};
        }
        return Collision{false, Vector2(0, 0)};
    }

    // if shape is a rect
    if (dynamic_cast<const Rectangle2D *>(&shape) != nullptr)
    {
        const Circle2D &circle = *this;
        Rectangle2D rect = dynamic_cast<const Rectangle2D &>(shape);

        float closestX = clamp(circle.position.x, rect.position.x, rect.position.x + rect.size.x);
        float closestY = clamp(circle.position.y, rect.position.y, rect.position.y + rect.size.y);

        float distanceX = circle.position.x - closestX;
        float distanceY = circle.position.y - closestY;
        float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);

        // If the distance is less than the circle's radius, they are colliding
        bool hasCollision = distanceSquared <= (circle.radius * circle.radius);
        if (!hasCollision)
        {
            return Collision{false, Vector2(0, 0)};
        }

        Vector2 normal = Vector2(100, 100);
        // Calculate the normal of the collision based on which side of the rectangle it occurs on
        float dx = circle.position.x - rect.position.x - rect.size.x / 2.0f;
        float dy = circle.position.y - rect.position.y - rect.size.y / 2.0f;

        // Determine the closest side of the rectangle
        float w = rect.size.x / 2.0f;
        float h = rect.size.y / 2.0f;
        float x_overlap = w - std::abs(dx);
        float y_overlap = h - std::abs(dy);

        if (x_overlap > y_overlap)
        {
            // Collision on the top or bottom side of the rectangle
            if (dy > 0)
            {
                normal = {0.0f, 1.0f}; // Collision on the top side
            }
            else
            {
                normal = {0.0f, -1.0f}; // Collision on the bottom side
            }
        }
        else
        {
            // Collision on the left or right side of the rectangle
            if (dx > 0)
            {
                normal = {1.0f, 0.0f}; // Collision on the right side
            }
            else
            {
                normal = {-1.0f, 0.0f}; // Collision on the left side
            }
        }

        lastCollisionTime[tuple] = CV::time();
        return Collision{true, normal};
    }

    // discrete circle
    int colliderResolution = 16;
    float stepSize = 2 * 3.1415 / colliderResolution;
    for (int i = 0; i < colliderResolution; i++)
    {
        float x = radius * cos(i * stepSize);
        float y = radius * sin(i * stepSize);
        if (shape.pointInside(position + Vector2(x, y)))
        {
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

Polygon2D::~Polygon2D()
{
    this->vertices.clear();
}

bool Polygon2D::pointInside(const Vector2 &point) const
{
    // Check for empty polygon
    unsigned int i, j;
    bool c = false;
    for (i = 0, j = vertices.size() - 1; i < vertices.size(); j = i++)
    {
        if (((vertices[i].y > point.y) != (vertices[j].y > point.y)) &&
            (point.x < (vertices[j].x - vertices[i].x) * (point.y - vertices[i].y) / (vertices[j].y - vertices[i].y) + vertices[i].x))
            c = !c;
    }
    return c;
}

Collision Polygon2D::intersects(const Shape2D &shape, bool ignoreCache) const
{
    // TODO consertar isso, por algum motivo n passa nos testes
    std::cout << "not implemented polygon collision" << std::endl;
    return Collision{false, Vector2(0, 0)};
    // for(auto &vert:vertices){
    //     if(shape.pointInside(vert)){
    //         std::cout << "normal not available for polygon collision" << std::endl;
    //         return Collision{true, Vector2(0, 0)};
    //     }
    // }
    // return Collision{false, Vector2(0, 0)};
}
