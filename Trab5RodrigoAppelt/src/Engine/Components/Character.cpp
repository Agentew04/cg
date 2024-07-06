#include "Character.h"

using namespace Engine::Components;

#include "Camera.h"
#include "../Engine.h"

void Character::Start(){
    // acha a camera
    auto cams = ::Engine::Engine::instance->GetAllComponentsOfType<Camera>();
    for(auto& cam: cams){
        if(cam->isActive){
            linkedCamera = cam;
        }
    }
}

void Character::Update(float delta){
    // move a posicao da camera
    if(linkedCamera){
        if((((int)mov) & (int)Movement::LEFT) > 0){
            linkedCamera->rotation.y = linkedCamera->rotation.y + delta;
        }
        if((((int)mov) & (int)Movement::RIGHT) > 0){
            linkedCamera->rotation.y = linkedCamera->rotation.y - delta;
        }
        if((((int)mov) & (int)Movement::UP) > 0){
            linkedCamera->rotation.x = linkedCamera->rotation.x + delta;
        }
        if((((int)mov) & (int)Movement::DOWN) > 0){
            linkedCamera->rotation.x = linkedCamera->rotation.x - delta;
        }

        Vector3 up = Vector3(0,1,0);
        Vector3 front = linkedCamera->direction();
        Vector3 right = up.cross(front);

        // move para frente automaticamente
        linkedCamera->position += front * (delta * forwardSpeed);
    }else{
        auto cams = ::Engine::Engine::instance->GetAllComponentsOfType<Camera>();
        for(auto& cam: cams){
            if(cam->isActive){
                linkedCamera = cam;
            }
        }
    }
}

void Character::KeyDown(int key){
    std::cout << "Character KeyDown: " << key << std::endl;
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
    }
    // ativa desativa o movimento
}