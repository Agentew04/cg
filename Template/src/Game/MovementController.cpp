#include "MovementController.h"

#include <iostream>

#define LEFT_ARROW 200
#define UP_ARROW 201
#define RIGHT_ARROW 202
#define DOWN_ARROW 203

#define LEFT_DIR 1
#define UP_DIR 2
#define RIGHT_DIR 4
#define DOWN_DIR 8

MovementController::MovementController(float maxSpeed, MovementType moveType){
    this->maxVelocity = maxSpeed;
    this->movementType = moveType;
    this->pressedKeys = 0;
    this->position = Vector2::zero();
}

void MovementController::update(float delta){
    position = position + (velocity*delta);
    std::cout << "Pos: " << position << " Vel" << velocity << std::endl;
}

static int keyToDir(int key, MovementType type){
    if((key == 'a' || key == 'A') && type == MovementType::WASD){
        return LEFT_DIR;
    }
    if(key == LEFT_ARROW && type == MovementType::ARROWS){
        return LEFT_DIR;
    }

    if((key == 'w' || key == 'W') && type == MovementType::WASD){
        return UP_DIR;
    }
    if(key == UP_ARROW && type == MovementType::ARROWS){
        return UP_DIR;
    }

    if((key == 'd' || key == 'D') && type == MovementType::WASD){
        return RIGHT_DIR;
    }
    if(key == RIGHT_ARROW && type == MovementType::ARROWS){
        return RIGHT_DIR;
    }

    if((key == 's' || key == 'S') && type == MovementType::WASD){
        return DOWN_DIR;
    }
    if(key == DOWN_ARROW && type == MovementType::ARROWS){
        return DOWN_DIR;
    }

    return 0;
}

void createVelocity(uint8_t keys, Vector2 *v, float magnitude){
    Vector2 vel = Vector2::zero();

    if(keys & LEFT_DIR){
        vel = vel + Vector2::left();
    }
    if(keys & UP_DIR){
        vel = vel + Vector2::up();
    }
    if(keys & RIGHT_DIR){
        vel = vel + Vector2::right();
    }
    if(keys & DOWN_DIR){
        vel = vel + Vector2::down();
    }

    if(vel != Vector2::zero())
        vel.normalize();
    *v = vel*magnitude;
}

void MovementController::keyDown(int key){

    int dir = keyToDir(key, this->movementType);
    pressedKeys |= dir;

    createVelocity(pressedKeys, &this->velocity, this->maxVelocity);

}

void MovementController::keyUp(int key){

    int dir = keyToDir(key, this->movementType);

    pressedKeys &= ~dir; // remove direction

    createVelocity(pressedKeys, &this->velocity, this->maxVelocity);
}

void MovementController::addPosition(Vector2 offset){
    this->position += offset;
}

void MovementController::setPosition(Vector2 pos){
    this->position = pos;
}

Vector2 MovementController::getPosition(){
    return this->position;
}

void MovementController::setMaxSpeed(float maxSpeed){
    this->maxVelocity = maxSpeed;
}
