#include "Character.h"

using namespace Engine::Components;

#include "../Engine.h"

void Character::Update(float delta){
    // move a posicao da camera

    if(auto act = actor.lock()){
        if((((int)mov) & (int)Movement::LEFT) > 0){
            act->rotation.y = act->rotation.y + delta;
        }
        if((((int)mov) & (int)Movement::RIGHT) > 0){
            act->rotation.y = act->rotation.y - delta;
        }
        if((((int)mov) & (int)Movement::UP) > 0){
            act->rotation.x = act->rotation.x + delta;
        }
        if((((int)mov) & (int)Movement::DOWN) > 0){
            act->rotation.x = act->rotation.x - delta;
        }

        Vector3 up = Vector3(0,1,0);
        Vector3 front = act->getForward();
        Vector3 right = up.cross(front);

        if((((int)mov) & (int)Movement::FRONT) > 0){
            act->position += front * (delta * forwardSpeed);
        }
    }
}

void Character::KeyDown(int key){
    switch(key){
        case 'a':
            mov = (Movement)((int)mov | (int)Movement::LEFT);
            break;
        case 'd':
            mov = (Movement)((int)mov | (int)Movement::RIGHT);
            break;
        case 'w':
            mov = (Movement)((int)mov | (int)Movement::UP);
            break;
        case 's':
            mov = (Movement)((int)mov | (int)Movement::DOWN);
            break;
        case ' ':
            mov = (Movement)((int)mov | (int)Movement::FRONT);
            break;
    }
}

void Character::KeyUp(int key){
    switch(key){
        case 'a':
            mov = (Movement)((int)mov & ~(int)Movement::LEFT);
            break;
        case 'd':
            mov = (Movement)((int)mov & ~(int)Movement::RIGHT);
            break;
        case 'w':
            mov = (Movement)((int)mov & ~(int)Movement::UP);
            break;
        case 's':
            mov = (Movement)((int)mov & ~(int)Movement::DOWN);
            break;
        case ' ':
            mov = (Movement)((int)mov & ~(int)Movement::FRONT);
            break;
    }
    // ativa desativa o movimento
}
