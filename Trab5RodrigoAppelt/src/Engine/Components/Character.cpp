#include "Character.h"

using namespace Engine::Components;

#include "../Engine.h"
#include <GL/glut.h>

void Character::Update(float delta){
    if(auto act = actor.lock()){
        if((((int)mov) & (int)Movement::LEFT) > 0){
            act->rotation.y = act->rotation.y + delta * sensitivity;
        }
        if((((int)mov) & (int)Movement::RIGHT) > 0){
            act->rotation.y = act->rotation.y - delta * sensitivity;
        }
        if((((int)mov) & (int)Movement::UP) > 0){
            act->rotation.x = act->rotation.x + delta * sensitivity;
        }
        if((((int)mov) & (int)Movement::DOWN) > 0){
            act->rotation.x = act->rotation.x - delta * sensitivity;
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
        case 'g':
            if(!wireframe){
                glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
                wireframe = true;
            }else{
                glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
                wireframe = false;
            }
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
}
