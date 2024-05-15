#include "Ball.h"

#include "../../gl_canvas2d.h"

void Ball::render()
{
    CV::translate(position);
    CV::color(1, 1, 1);
    CV::circleFill(0, 0, radius, 16);

    #if PHYSICS_DEBUG
    // ball collider
    CV::color(1,0,0);
    CV::circle(collider.position-position, 10, 16);
    // direction
    CV::color(0,0,1);
    CV::line(collider.position-position, (collider.position+velocity.normalized()*collider.radius*4)-position, 2.0f);
    // ball position
    CV::color(1,1,0);
    CV::circleFill(Vector2::zero(), 4, 8);
    #endif
}