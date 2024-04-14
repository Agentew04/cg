#ifndef __GAME_H__
#define __GAME_H__

#include <vector>

#include "../Math/Vector2.h"
#include "../Keyboard.h"

#include "Components/Ball.h"
#include "Components/Block.h"
#include "Components/Powerups.h"

/// @brief Classe que vai guardar o estado do jogo.
/// Nivel, blocos quebrados, bolas etc
class Game {
public:
    /// @brief Cria uma instancia de um jogo
    Game(int *scrW, int *scrH);
    /// @brief Libera recursos usados pelo jogo
    ~Game();

    /// @brief Reinicia todo o estado do jogo
    void reset();

    // event forwarding
    void update(float delta);
    void render();
    void keyDown(Key key);
    void mouseUp();
    void mouseDown();
    void updateMousePos(Vector2 pos);
private:
    int *screenWidth;
    int *screenHeight;
    Vector2 mousePos;

    Vector2 gameAreaStart;
    Vector2 gameAreaSize;

    void renderHeader();
    void renderGameArea();
    void renderArrow();

    bool hasActivePlay;
    const float burstInterval = 0.5f;
    int burstCount;
    int level;

    std::vector<Ball> balls;
    std::vector<std::vector<Block>> blockLines;
    std::vector<Powerup> powerups;

    Vector2 ballLaunchPosition;
};

#endif