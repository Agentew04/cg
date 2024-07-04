#include "Character.h"

using namespace Engine::Components;

void Character::Start(){
    // acha a camera
}

void Character::Update(float delta){
    // move a posicao da camera

    if(linkedCamera){
        Vector3 up = Vector3(0,1,0);
        Vector3 front = linkedCamera->direction();
        Vector3 right = up.cross(front);
        if(((int)mov) & (int)Movement::LEFT > 0){
            linkedCamera->position += -right * delta;
        }else if(((int)mov) & (int)Movement::RIGHT > 0){
            linkedCamera->position += right * delta;
        }
    }
}

void Character::Key(int key){
    switch(key){
        case 'a':
            mov = (Movement)((int)mov | (int)Movement::LEFT);
            break;
        case 'd':
            mov = (Movement)((int)mov | (int)Movement::RIGHT);
            break;
    }
    // ativa desativa o movimento
}