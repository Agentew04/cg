#ifndef __TEXT_BOX_H__
#define __TEXT_BOX_H__

#include <functional>
#include <map>

#include "UIDefinitions.h"
#include "../Keyboard.h"
#include "../Math/Vector2.h"
#include "../Math/Vector3.h"

/// @brief Classe que representa uma caixa de texto na aplicacao.
class TextBox {
public:
    class Style{
    public:
        std::map<UIState, Vector3> backgroundColor;
        std::map<UIState, Vector3> borderColor;
        Vector3 foreground;

        void setState(UIState state, Vector3 background, Vector3 border);
        void getState(UIState state, Vector3 *background, Vector3 *border);

        static void startUsing();
        static void freeStyles();
        static Style* FlatWhite();
    private:
        static int styleObserverCount;
        static Style *flatWhiteStyle;
    };

    TextBox(
        std::function<Vector2()> positionFunc,
        std::function<Vector2()> sizeFunc,
        UIPlacement placement = UIPlacement::CENTER,
        TextAlign textAlign = TextAlign::LEFT
    );

    Style *style;

    // event forwarding
    void render();
    void keyDown(Key key);
    void mouseDown();
    void mouseUp();
    void updateMousePos(Vector2 pos);

    std::string getText();
private:
    Vector2 mousePos;
    std::function<Vector2()> positionFunc;
    std::function<Vector2()> sizeFunc;
    UIPlacement placement;
    TextAlign textAlign;

    UIState currentState;

    std::string text;

    static bool inside(Vector2 pos, Vector2 size, Vector2 mousePos);
};

#endif