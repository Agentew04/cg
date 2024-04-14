#include "Block.h"

Block::Block() :
    position(Vector2::zero()),
    color(Vector3::zero()),
    life(1){}

Block::Block(Vector2 pos, Vector3 color, int life) :
    position(pos),
    color(color),
    life(life){}