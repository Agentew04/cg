#ifndef __CHECKBOX_H__
#define __CHECKBOX_H__

#include <map>

#include "../Vector2.h"
#include "../Vector3.h"

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


    Checkbox(Vector2 pos, Vector2 size, std::string label, bool defaultValue);
    ~Checkbox();

    void draw();

    Vector2 getPos();
    Vector2 getSize();

    Style *style;
    State state;

    bool getValue();
    void setValue(bool value);
    void setBinding(bool *target);
private:
    Vector2 pos;
    Vector2 sz;
    bool value;
    bool *bindingTarget;
    std::string label;
};

#endif // __CHECKBOX_H__
