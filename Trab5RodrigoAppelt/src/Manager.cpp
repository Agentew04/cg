#include "Manager.h"

#include <memory>

#include "Engine/Actor.h"
#include "Engine/Components/Skybox.h"


void Manager::startloop(){
    ::Engine::Engine::instance = &engine;
    Engine::Actor skyboxActor;
    skyboxActor.components.push_back(std::move(std::make_shared<Engine::Components::Skybox>()));
    engine.hierarchy.push_back(std::move(skyboxActor));

    engine.Start();
}

void Manager::render(){
    engine.Render();
}

void Manager::update(float delta){
    engine.Update(delta);
}

void Manager::keyboard(Key key){
    
}
