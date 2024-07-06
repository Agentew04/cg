#include "Manager.h"

#include <memory>

#include "Engine/Actor.h"
#include "Engine/Components/Skybox.h"
#include "Engine/Components/Camera.h"
#include "Engine/Components/Character.h"


void Manager::startloop(){
    ::Engine::Engine::instance = &engine;
    Engine::Actor skyboxActor;
    skyboxActor.components.push_back(std::move(std::make_shared<Engine::Components::Skybox>()));
    engine.hierarchy.push_back(std::move(skyboxActor));

    Engine::Actor cameraActor;
    std::shared_ptr<Engine::Components::Camera> cam = std::make_shared<Engine::Components::Camera>();
    cam->isActive = true;
    cameraActor.components.push_back(std::move(cam));
    cameraActor.components.push_back(std::move(std::make_shared<Engine::Components::Character>()));
    engine.hierarchy.push_back(std::move(cameraActor));

    engine.Start();
}

void Manager::render(){
    engine.Render();
}

void Manager::update(float delta){
    engine.Update(delta);
}

void Manager::keyboard(Key key){
    std::cout << "Manager KeyDown: " << key << std::endl;
    engine.KeyDown(key);
}

void Manager::keyboardUp(Key key){
    engine.KeyUp(key);
}
