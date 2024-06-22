#ifndef __SLIDER_H__
#define __SLIDER_H__

#include <map>
#include <functional>

#include "../Math/Vector2.h"
#include "../Math/Vector3.h"

// classe que representa um slider
class Slider{
public:

    enum State {
        NORMAL,
        HOVER,
        CLICK
    };

    // struct que contem todas as informacoes de cor sobre um botao
    struct Style{
        std::map<State, Vector3> handleColor;

        void setState(State state, Vector3 handle);
        void getState(State state, Vector3 *handle);

        Vector3 backSliderColor;
        Vector3 outSliderColor;
        Vector3 tickColor;

        static Style* Windows10();

        // libera recursos internos do estilo(mapas)
        ~Style();
    };


    Slider(
        std::function<Vector2()> posFunc, 
        std::function<Vector2()> sizeFunc, 
        float minValue = 0.0f, 
        float maxValue = 1.0f, 
        float defaultValue = 0.5f);
    ~Slider();

    void draw();

    Vector2 getPos();
    Vector2 getSize();

    Style *style;
    State state = State::NORMAL;

    void getValueBounds(float *minValue, float *maxValue);
    float getValue();
    void setValue(float value, bool notify = true);

    void setBinding(float *target);
    void setCallback(std::function<void(float)> callback);
private:
    std::function<Vector2()> posFunc;
    std::function<Vector2()> sizeFunc;
    float minValue;
    float maxValue;
    float value;

    float *bindingTarget = nullptr;
    bool hasCallback = false;
    std::function<void(float)> callback;
};



#endif // __SLIDER_H__
