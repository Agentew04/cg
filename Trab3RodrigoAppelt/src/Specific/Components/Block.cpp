#include "Block.h"

#include "../../gl_canvas2d.h"

void Block::render()
{
    CV::color(1, 0, 0);
    CV::rectFill(0, 0, 10, 10);
}