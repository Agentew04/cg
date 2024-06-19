#ifndef __PROGRESS_BAR_H__
#define __PROGRESS_BAR_H__

#include <vector>

#include "../Math/Vector2.h"
#include "UIDefinitions.h"

/// @brief 
class ProgressRing {
public:
    ProgressRing(Vector2 pos,
        float radius,
        int resolution = 100);

    void render();
    void setValue(float value);
private:
    std::vector<Vector2> vertices;
    float value;
    Vector2 pos;
};

#endif // __PROGRESS_BAR_H__