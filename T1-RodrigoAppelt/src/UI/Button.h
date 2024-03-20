#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <string>
#include <functional>
#include <map>

#include "../Vector2.h"
#include "../Vector3.h"

// enum que representa o estado que um botao pode estar
enum ButtonState {
    NORMAL,
    HOVER,
    CLICK
};

// struct que contem todas as informacoes de cor sobre um botao
struct ButtonStyle{
    std::map<ButtonState, Vector3> backgroundColor;
    std::map<ButtonState, Vector3> borderColor;

    void setState(ButtonState state, Vector3 background, Vector3 border);

    void getState(ButtonState state, Vector3 *background, Vector3 *border);

    Vector3 foreground;

    static ButtonStyle* Windows10();

    ~ButtonStyle();
};

// classe que representa um botao na aplicacao, com uma callback
class Button{
public:
    Button(Vector2 pos, Vector2 sz, std::string text, std::function<void()> callback);
    ~Button();

    void draw();

    void call();

    bool clickable;

    ButtonStyle *style;
    ButtonState state;

    Vector2 getPos();
    Vector2 getSize();


private:


    Vector2 pos;
    Vector2 sz;
    std::string text;
    std::function<void()> callback;
};



#endif // __BUTTON_H__
