#ifndef __PROGRESS_BAR_H__
#define __PROGRESS_BAR_H__

#include <functional>

#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#include "UIDefinitions.h"

class ProgressBar {
public:
    ProgressBar(
        std::function<Vector2()> position = [](){return Vector2::zero();},
        std::function<Vector2()> size = [](){return Vector2::zero();},
        Vector3 background = Vector3::zero(),
        Vector3 foreground = Vector3::zero(),
        UIPlacement placement = UIPlacement::CENTER,
        float minValue = 0, float maxValue = 100);
    void render();

    void setValue(float value);
    float getValue();

    void setMaxValue(float value);
private:
    std::function<Vector2()> positionFunc;
    std::function<Vector2()> sizeFunc;

    float minValue;
    float maxValue;
    float currentValue;

    Vector3 background;
    Vector3 foreground;
};

#endif
