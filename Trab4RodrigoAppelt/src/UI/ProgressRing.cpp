#include "ProgressRing.h"

#include <cmath>
#include <algorithm>

#include "../gl_canvas2d.h"

ProgressRing::ProgressRing(Vector2 pos, float radius, int resolution) : 
    value(0.0f),
    pos(pos)
{
    vertices.reserve(resolution+2);
    vertices.push_back(Vector2(pos));
    float stepSize = 2 * 3.14159265358979323846 / resolution;
    for (int i = 0; i < resolution+1; i++)
    {
        float x = radius * cos(i * stepSize);
        float y = radius * sin(i * stepSize);
        vertices.push_back(Vector2(pos.x + x, pos.y + y));
    }
}

void ProgressRing::render()
{
    CV::translate(Vector2::zero());
    int vert = vertices.size()-2;

    vert = (int)std::round(vert * value);
    CV::triangleFan(vertices, vert+2);
}

void ProgressRing::setValue(float value)
{
    if(value < 0.0f){
        value = 0.0f;
    }
    if(value > 1.0f){
        value = 1.0f;
    }
    this->value = value;
}