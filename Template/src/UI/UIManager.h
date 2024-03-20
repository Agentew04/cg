#ifndef __UI_MANAGER_H__
#define __UI_MANAGER_H__

#include <vector>

#include "Button.h"
#include "Slider.h"
#include "CheckBox.h"

#include "ButtonManager.h"
#include "SliderManager.h"
#include "CheckBoxManager.h"
#include "Chart.h"

// Classe para agregar gerenciadores de multiplas entidades
class UIManager {
public:
    void add(Button *button);
    void remove(Button *button);
    void add(Slider *slider);
    void remove(Slider *slider);
    void add(Checkbox *checkbox);
    void remove(Checkbox *checkbox);
    void add(Chart *chart);
    void remove(Chart *chart);

    UIManager();
    ~UIManager();

    void draw();
    void updateMousePos(Vector2 mousePos);
    void mouseDown();
    void mouseUp();
private:

    ButtonManager buttonMngr;
    SliderManager sliderMngr;
    CheckboxManager checkboxMngr;
    std::vector<Chart*> charts;
    Vector2 mousePos;
};

#endif // __UI_MANAGER_H__
