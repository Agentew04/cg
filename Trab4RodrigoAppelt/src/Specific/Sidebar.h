#ifndef __SIDEBAR_H__
#define __SIDEBAR_H__

#include "../Math/Vector2.h"

#include "../UI/Slider.h"
#include "../UI/SliderManager.h"
#include "../UI/ButtonManager.h"
#include "../UI/CheckBoxManager.h"

class Manager;

/// @brief Classe para gerenciar a exibicao a barra lateral de
/// controles do trabalho.
class Sidebar {
public:
    Sidebar(int* screenWidth, int* screenHeight, Manager* manager);

    // functions
    float getRpm();
    float getDriveshaftAngle();
    float getAmbientLight();
    float getUpscaleFactor();

    // ui
    float getSidebarWidth();

    // event forwarding
    void update(float delta);
    void render();
    void updateMousePos(Vector2 mousePos);
    void mouseDown();
    void mouseUp();
private:
    int *scrW, *scrH;
    Vector2 mousePos;
    Manager* manager;

    // tamanhos
    int sidebarWidth = 250;
    Vector3 sidebarColor = Vector3::fromHex(0x219ebc);
    int margin = 5;

    // funcoes
    void submitUI();

    // componentes
    Slider *rpmSlider = nullptr;
    Slider *driveshaftAngleSlider = nullptr;
    Slider *ambientLightSlider = nullptr;
    Slider *upscaleSlider = nullptr;
    SliderManager sliderManager;
    ButtonManager buttonManager;
    CheckboxManager checkboxManager;
};

#endif