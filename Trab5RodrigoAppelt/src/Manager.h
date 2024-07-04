#ifndef __MANAGER_H__
#define __MANAGER_H__

#include "Keyboard.h"
#include "Engine/Engine.h"

/// @brief Gerencia toda a interacao com a engine e o usuario
class Manager {
public:

    void startloop();
    void render();
    void update(float delta);
    void keyboard(Key key);
private:
    Engine::Engine engine;
};

#endif