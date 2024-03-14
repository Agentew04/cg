#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <string>
#include <functional>

#include "../Vector2.h"
#include "../Vector3.h"

// o clique do botao vai ser ativado no press up

class Button{
public:
    Button(Vector2 pos, Vector2 sz, std::string text, bool relative, std::function<void()> callback);
    ~Button();

    void draw();

    void call();

    bool clickable;

    Vector3 background;
    Vector3 foreground;
    Vector3 borderColor;
    Vector3 hoverColor;
    Vector3 clickColor;
private:

    Vector2 pos;
    Vector2 sz;
    std::string text;
    std::function<void()> callback;
};

#endif // __BUTTON_H__
