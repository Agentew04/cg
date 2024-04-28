#ifndef __GAME_H__
#define __GAME_H__

#include <vector>
#include <random>

#include "../Math/Vector2.h"
#include "../Keyboard.h"
#include "../Misc/ParticleManager.h"

#include "Components/Ball.h"
#include "Components/Block.h"
#include "Components/Powerups.h"

/// @brief Classe que vai gerenciar e
/// guardar o estado do jogo.
/// Nivel, blocos quebrados, bolas etc
class Game {
public:
    /// @brief Cria uma instancia de um jogo
    Game(int *scrW, int *scrH);
    /// @brief Libera recursos usados pelo jogo
    ~Game();

    /// @brief Reinicia todo o estado do jogo
    void reset();

    bool isGameOver();

    // event forwarding
    void update(float delta);
    void render();
    void keyDown(Key key);
    void keyUp(Key key);
    void mouseUp();
    void mouseDown();
    void updateMousePos(Vector2 pos);
private:
    std::mt19937 rng;
    ParticleManager particleManager;

    int *screenWidth;
    int *screenHeight;
    Vector2 mousePos;

    Vector2 gameAreaStart;
    Vector2 gameAreaSize;
    // left, right, ceiling
    std::vector<Rectangle2D> gameAreaWalls;
    void updateGameAreaBounds();

    const Vector2 blockAreaSize = Vector2(7,10);

    void renderHeader();
    void renderGameArea();
    void renderLine(std::vector<Block> line);
    void renderArrow();

    bool hasActivePlay;
    const float burstInterval = 0.2f;
    uint32_t burstCount;
    uint32_t spawned;
    float lastBurstTime;
    int level;
    bool firstBack;
    bool gameOver;

    std::vector<Ball> balls;
    std::vector<std::vector<Block>> blockLines;
    std::vector<Powerup> powerups;
    std::vector<Vector3> blockColors;

    Vector2 ballLaunchPosition;
    Vector2 ballLaunchDirection;
    Vector2 nextLaunchPosition;
    float ballSpeed;
    float ballSpeedMultiplier;

    std::vector<Block> createNewLine(int level);
    void pushLines();

    // funcoes para gerenciar a colisao melhor
    Block* getBlockAt(Vector2 pos);
    bool processCollisions(Ball& ball, Block& block);
};

#endif
