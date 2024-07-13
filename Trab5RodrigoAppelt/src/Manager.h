#ifndef __MANAGER_H__
#define __MANAGER_H__

#include "Keyboard.h"
#include "Engine/Engine.h"
#include "TreeCreator.h"
#include "Engine/Components/TerrainBezier.h"
#include "Engine/Components/Character.h"

/// @brief Gerencia toda a interacao com a engine e o usuario.
/// Vai ter o codigo do jogo.
class Manager {
public:

    void startloop();
    void render();
    void update(float delta);
    void keyboard(Key key);
    void keyboardUp(Key key);
private:
    Engine::Engine engine;
    TreeCreator treeCreator;
    std::shared_ptr<Engine::Components::TerrainBezier> terrain;
    std::shared_ptr<Engine::Components::Character> player;

    void createWorld();

    void createTerrain();
    void createTrees();
    void createCharacter();
};

#endif