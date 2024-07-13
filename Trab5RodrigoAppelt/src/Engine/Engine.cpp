#include "Engine.h"

#include "Actor.h"
#include "Components/Camera.h"

#include <GL/glut.h>

Engine::Engine* Engine::Engine::instance = nullptr;

std::optional<std::shared_ptr<Engine::Actor>> Engine::Engine::FindActorByName(const std::string& name){
    for (auto actor : hierarchy){
        auto ret = _FindActorByName(actor, name);
        if (ret.has_value()){
            return ret;
        }
    }
    return std::nullopt;
}

std::optional<std::shared_ptr<Engine::Actor>> Engine::Engine::_FindActorByName(std::shared_ptr<Actor> root, const std::string& name){
    if (root->name == name){
        return root;
    }
    for (auto child : root->children){
        auto ret = _FindActorByName(child, name);
        if (ret.has_value()){
            return ret;
        }
    }
    return std::nullopt;
}

void Engine::Engine::Start(){
    for (auto actor : hierarchy){
        actor->Start();
    }
}

void Engine::Engine::Update(float delta){
    // update camera look at
    auto cams = GetAllComponentsOfType<Components::Camera>();
    for(auto& cam: cams){
        if(cam->isActive){
            if(auto act = cam->actor.lock()){
                cameraPos = act->getGlobalPosition();
                cameraLookAt = act->getForward();
                std::cout << "Camera at " << cameraPos << " looking at " << act->rotation << std::endl;
            }
        }
    }

    for (auto actor : hierarchy){
        actor->Update(delta);
    }
}

void Engine::Engine::Destroy(){
    for (auto actor : hierarchy){
        actor->Destroy();
    }
    hierarchy.clear();
}

void Engine::Engine::Render(){
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, // from. Posicao onde a camera esta posicionada
              cameraPos.x + cameraLookAt.x, cameraPos.y + cameraLookAt.y, cameraPos.z + cameraLookAt.z,  // to. Posicao absoluta onde a camera esta vendo
              0, 1, 0); // up. Vetor Up.

    // light
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0f };
    GLfloat light_diffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLfloat light_position[] = { -1.0f, 1.0f, 1.0f, 0.0f };
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    for (auto actor : hierarchy){
        actor->Render();
    }

    glutSwapBuffers();
}

void Engine::Engine::KeyDown(int key){
    for (auto actor : hierarchy){
        actor->KeyDown(key);
    }
}

void Engine::Engine::KeyUp(int key){
    for (auto actor : hierarchy){
        actor->KeyUp(key);
    }
}