#include "Ball.h"

#include "../../gl_canvas2d.h"

void Ball::render()
{
    CV::translate(position);
    CV::color(1, 1, 1);
    CV::circleFill(0, 0, radius, 16);
}