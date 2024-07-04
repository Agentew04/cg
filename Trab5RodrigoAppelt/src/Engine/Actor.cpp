#include "Actor.h"

using namespace Engine;

void Actor::Start(){
    for (auto& component : components){
        component->Start();
    }
}

void Actor::Update(float delta){
    for (auto& component : components){
        component->Update(delta);
    }
    for (auto& child : children){
        child.Update(delta);
    }
}

void Actor::Destroy(){
    for (auto& child : children){
        child.Destroy();
    }
    for (auto& component : components){
        component->Destroy();
    }

    components.clear();
    children.clear();
}

void Actor::Render(){
    for (auto& component : components){
        component->Render();
    }
    for (auto& child : children){
        child.Render();
    }
}
