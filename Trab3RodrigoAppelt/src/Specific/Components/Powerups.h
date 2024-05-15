#ifndef __POWERUPS_H__
#define __POWERUPS_H__

#include "../../Math/Vector2.h"
#include "../../Math/Polygon2D.h"

class ExtraBall {
public:
    Vector2 position;
    ExtraBall(Vector2 pos);
    void render(float blockSize, Vector2 gameAreaStart);
    Circle2D collider;
};

class Laser {
public:
    enum class Direction{
        Horizontal,
        Vertical
    };

    Laser(Vector2 pos, Direction dir);
    Direction direction;
    Vector2 position;
    bool dirty;
    float *blockSize;
    Vector2 *gameAreaStart;
    void render(float blockSize, Vector2 gameAreaStart);
    void activateVFX(float blockSize, Vector2 gameAreaStart, Vector2 gameAreaSize);
    Rectangle2D collider;
};

#endif // __POWERUPS_H__
