#ifndef __CHECKBOX_H__
#define __CHECKBOX_H__

#include <map>
#include <functional>

#include "../Math/Vector2.h"
#include "../Math/Vector3.h"

// classe que representa um slider
class Checkbox{
public:

    enum State {
        NORMAL,
        HOVER,
        CLICK
    };

    // struct que contem todas as informacoes de cor sobre um botao
    struct Style{
        std::map<State, Vector3> backgroundColor;
        std::map<State, Vector3> borderColor;
        std::map<State, Vector3> checkColor;

        void setState(State state, Vector3 background, Vector3 border, Vector3 check);
        void getState(State state, Vector3 *background, Vector3 *border, Vector3 *check);

        Vector3 foregroundColor;

        static Style* Windows10();

        // libera recursos internos do estilo(mapas)
        ~Style();
    };


    Checkbox(
        std::function<Vector2()> posFunc, 
        std::function<Vector2()> sizeFunc, 
        std::string label, 
        bool defaultValue = false);
    ~Checkbox();

    void draw();

    Vector2 getPos();
    Vector2 getSize();

    Style *style;
    State state = State::NORMAL;

    bool getValue();
    void setValue(bool value, bool notify = true);
    void setBinding(bool *target);

    void setCallback(std::function<void(bool)> callback);
private:
    std::function<Vector2()> posFunc;
    std::function<Vector2()> sizeFunc;
    bool value;
    bool *bindingTarget = nullptr;
    std::string label;
    bool hasCallback = false;
    std::function<void(bool)> callback;
};

#endif // __CHECKBOX_H__
