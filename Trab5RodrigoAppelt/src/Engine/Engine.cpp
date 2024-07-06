#include "Engine.h"

#include "Actor.h"
#include "Components/Camera.h"

#include <GL/glut.h>

Engine::Engine* Engine::Engine::instance = nullptr;

std::optional<std::reference_wrapper<Engine::Actor>> Engine::Engine::FindActorByName(const std::string& name){
    for (auto actor : hierarchy){
        auto ret = _FindActorByName(actor, name);
        if (ret.has_value()){
            return ret;
        }
    }
    return std::nullopt;
}

std::optional<std::reference_wrapper<Engine::Actor>> Engine::Engine::_FindActorByName(Actor& root, const std::string& name){
    if (root.name == name){
        return root;
    }
    for (auto child : root.children){
        auto ret = _FindActorByName(child, name);
        if (ret.has_value()){
            return ret;
        }
    }
    return std::nullopt;
}

void Engine::Engine::Start(){
    for (auto actor : hierarchy){
        actor.Start();
    }
}

void Engine::Engine::Update(float delta){
    // update camera look at
    auto cams = GetAllComponentsOfType<Components::Camera>();
    for(auto& cam: cams){
        if(cam->isActive){
            cameraPos = cam->position;
            cameraLookAt = cam->direction();
        }
    }

    for (auto actor : hierarchy){
        actor.Update(delta);
    }
}

void Engine::Engine::Destroy(){
    for (auto actor : hierarchy){
        actor.Destroy();
    }
    hierarchy.clear();
}

void Engine::Engine::Render(){
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, // from. Posicao onde a camera esta posicionada
              cameraPos.x + cameraLookAt.x, cameraPos.y + cameraLookAt.y, cameraPos.z + cameraLookAt.z,  // to. Posicao absoluta onde a camera esta vendo
              0, 1, 0); // up. Vetor Up.

    for (auto actor : hierarchy){
        actor.Render();
    }

    glutSwapBuffers();
}

void Engine::Engine::KeyDown(int key){
    for (auto actor : hierarchy){
        actor.KeyDown(key);
    }
}

void Engine::Engine::KeyUp(int key){
    for (auto actor : hierarchy){
        actor.KeyUp(key);
    }
}