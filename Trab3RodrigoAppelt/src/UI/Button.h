#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <string>
#include <functional>
#include <map>

#include "../Math/Vector2.h"
#include "../Math/Vector3.h"

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
    static ButtonStyle* FlatLightBlue();
    static ButtonStyle* FlatDarkBlue();
    static ButtonStyle* FlatWhite();
    static ButtonStyle* FlatGreen();
    static ButtonStyle* FlatRed();

    /// @brief Registra mais um observador dos estilos.
    static void startUsing();
    /// @brief Libera todos os estilos compartilhados de botoes
    /// se nao houver mais observadores
    static void freeStyles();

    ~ButtonStyle();
};

// classe que representa um botao na aplicacao, com uma callback
class Button{
public:
    Button(Vector2 pos, Vector2 sz, std::string text, std::function<void(Button*)> callback);
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
    std::function<void(Button*)> callback;
};



#endif // __BUTTON_H__
