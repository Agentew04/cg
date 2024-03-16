#ifndef __SLIDER_H__
#define __SLIDER_H__

#include <map>

#include "../Vector2.h"
#include "../Vector3.h"

// classe que representa um slider
class Slider{
public:

    enum Orientation {
        HORIZONTAL,
        VERTICAL
    };

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


    Slider(Vector2 pos, Vector2 size, float minValue, float maxValue, float defaultValue, Orientation orientation);
    ~Slider();

    void draw();

    Vector2 getPos();
    Vector2 getSize();

    Style *style;
    State state;

    void getValueBounds(float *minValue, float *maxValue);
    float getValue();
    void setValue(float value);
private:
    Vector2 pos;
    Vector2 sz;
    Orientation orientation;
    float minValue;
    float maxValue;
    float value;
};



#endif // __SLIDER_H__
