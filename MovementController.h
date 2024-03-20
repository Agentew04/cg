#ifndef __MOVEMENT_CONTROLLER_H__
#define __MOVEMENT_CONTROLLER_H__

/*
* Autor: Rodrigo Appelt, 2024
*
* Um controlador de movimento com o teclado para pontos 2D
*/

#include "../Vector2.h"

enum MovementType {
    ARROWS = 1,
    WASD = 2
};

// Classe usada para gerenciar a movimentacao com teclado
// de um ponto arbitrario
class MovementController{
public:

    MovementController(float maxSpeed, MovementType moveType);

    // must be called every frame
    void update(float delta);

    // must be called when a key is pressed
    void keyDown(int key);

    // must be called when a key is released
    void keyUp(int key);

    // adds a offset to the position
    void addPosition(Vector2 offset);

    // overrides actual position
    void setPosition(Vector2 pos);

    // gets the current tracked position
    Vector2 getPosition();

    // defines the speed that the movement occurs
    void setMaxSpeed(float maxSpeed);

    // how the user handles movement
    MovementType movementType;
private:

    float maxVelocity = 1;
    uint8_t pressedKeys = 0;
    Vector2 velocity;
    Vector2 position;
};

#endif // __MOVEMENT_CONTROLLER_H__
