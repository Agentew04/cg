#ifndef __BALL_H__
#define __BALL_H__

#include "../../Math/Vector2.h"
#include "../../Math/Polygon2D.h"

class Ball {
public:
    void render();

    Vector2 position;
    Vector2 velocity;
    Circle2D collider;

    constexpr static const float radius = 5.0f;
private:
};

#endif // __BALL_H__
