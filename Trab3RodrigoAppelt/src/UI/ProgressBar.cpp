#include "ProgressBar.h"

#include "../gl_canvas2d.h"

ProgressBar::ProgressBar(
    std::function<Vector2()> position,
    std::function<Vector2()> size,
    Vector3 background,
    Vector3 foreground,
    UIPlacement,
    float minValue, float maxValue){
    this->minValue = minValue;
    this->maxValue = maxValue;
    this->currentValue = minValue;
    this->positionFunc = position;
    this->sizeFunc = size;
    this->background = background;
    this->foreground = foreground;
}

void ProgressBar::render(){
    CV::translate(positionFunc());

    Vector2 size = sizeFunc();
    CV::color(background);
    CV::rectFill(Vector2::zero(), sizeFunc());

    CV::color(foreground);
    CV::rectFill(Vector2::zero(), Vector2(size.x * (currentValue - minValue) / (maxValue - minValue), size.y));
}

void ProgressBar::setValue(float value){
    currentValue = value;
}

float ProgressBar::getValue(){
    return currentValue;
}

void ProgressBar::setMaxValue(float value){
    maxValue = value;
}
