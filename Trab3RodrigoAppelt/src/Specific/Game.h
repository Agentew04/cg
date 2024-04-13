#ifndef __GAME_H__
#define __GAME_H__

#include "../Math/Vector2.h"

/// @brief Classe que vai guardar o estado do jogo.
/// Nivel, blocos quebrados, bolas etc
class Game {
public:
    /// @brief Cria uma instancia de um jogo
    Game();
    /// @brief Libera recursos usados pel jogo
    ~Game();

    void update(float delta);
    void render();
    void mouseUp();
    void mouseDown();
    void updateMousePos(Vector2 pos);
private:
    Vector2 mousePos;
};

#endif