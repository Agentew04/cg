#include "Actor.h"

using namespace Engine;

void Actor::Start(){
    for (auto component : components){
        component.Start();
    }
}

void Actor::Update(){
    for (auto component : components){
        component.Update();
    }
    for (auto child : children){
        child.Update();
    }
}

void Actor::Destroy(){
    for (auto child : children){
        child.Destroy();
    }
    for (auto component : components){
        component.Destroy();
    }
}

void Actor::Render(){
    for (auto component : components){
        component.Render();
    }
    for (auto child : children){
        child.Render();
    }
}