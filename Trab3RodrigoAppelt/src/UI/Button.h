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

/// @brief Classe que representa um botao na aplicacao, com uma callback
class Button {
public:
    enum class ButtonPlacement{
        TOP_LEFT,
        LEFT,
        BOTTOM_LEFT,
        TOP_CENTER,
        CENTER,
        BOTTOM_CENTER,
        TOP_RIGHT,
        RIGHT,
        BOTTOM_RIGHT
    };

    Button(
        std::function<Vector2()> positionFunc,
        std::function<Vector2()> sizeFunc, 
        ButtonPlacement placement,
        std::string text, 
        std::function<void(Button*)> callback);
    ~Button();

    void render();

    void call();

    bool clickable;

    bool inside(Vector2 mousePos);

    ButtonStyle *style;
    ButtonState state;
private:
    std::function<Vector2()> sizeFunc;
    std::function<Vector2()> positionFunc;
    ButtonPlacement placement;

    std::string text;
    std::function<void(Button*)> callback;

    /// @brief Traduz coordenadas de origem variada para as coordenadas normais
    void translate(Vector2& pos, Vector2& size, ButtonPlacement placement);
};



#endif // __BUTTON_H__
