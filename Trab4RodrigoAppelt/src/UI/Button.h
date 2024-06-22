#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <string>
#include <functional>
#include <map>

#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#include "UIDefinitions.h"

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

    private:
    static int styleObserverCount;
    static ButtonStyle *windows10Style;
    static ButtonStyle *flatLightBlueStyle;
    static ButtonStyle *flatDarkBlueStyle;
    static ButtonStyle *flatWhiteStyle;
    static ButtonStyle *flatGreenStyle;
    static ButtonStyle *flatRedStyle;
};

/// @brief Classe que representa um botao na aplicacao, com uma callback
class Button {
public:

    Button(
        std::function<Vector2()> positionFunc,
        std::function<Vector2()> sizeFunc,
        UIPlacement placement,
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
    std::function<Vector2()> positionFunc;
    std::function<Vector2()> sizeFunc;
    UIPlacement placement;

    std::string text;
    std::function<void(Button*)> callback;

    float findPt();

};



#endif // __BUTTON_H__
