#ifndef __BLOCK_H__
#define __BLOCK_H__

#include "../../Math/Polygon2D.h"
#include "../../Math/Vector2.h"
#include "../../Math/Vector3.h"

class Block {
public:
    Block();
    Block(Vector2 pos, Vector3 color, int life);

    // render de block n vai ficar aqui pois depende muito
    // da area de jogo. movido para Game.cpp
    //void render();

    /// @brief Posicao inteira no campo de blocos
    Vector2 position;
    Vector3 color;

    int life;
    Rectangle2D collider;
private:
};

#endif // __BLOCK_H__