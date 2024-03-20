#ifndef __SLIDER_MANAGER_H__
#define __SLIDER_MANAGER_H__

#include <vector>

#include "Slider.h"

// manages many instances of sliders on the canvas
class SliderManager {
public:
    void registerSlider(Slider *button);
    void unregisterSlider(Slider *button);

    SliderManager();
    ~SliderManager();

    void draw();
    void updateMousePos(Vector2 mousePos);
    void mouseDown();
    void mouseUp();
private:
    Vector2 mousePos;
    Slider* holdingSlider;
    std::vector<Slider*> sliders;
};

#endif // __SLIDER_MANAGER_H__
