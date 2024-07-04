#include "Engine.h"

#include "Actor.h"

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

void Engine::Engine::Update(float delta){
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
    for (auto actor : hierarchy){
        actor.Render();
    }
}
