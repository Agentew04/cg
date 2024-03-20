#ifndef __CHART_H__
#define __CHART_H__

#include <vector>
#include <map>

#include "../Vector2.h"
#include "../Vector3.h"

class Chart {
public:
    void draw();

    Chart(Vector2 pos, Vector2 size);
    ~Chart();

    std::vector<float> x;
    std::vector<float> y;
    Vector2 pos,size;
    Vector3 color;
    Vector2 xBounds, yBounds;
private:
};

#endif // __CHART_H__
