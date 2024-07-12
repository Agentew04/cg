#include "Manager.h"

#include <memory>

#include "Engine/Actor.h"
#include "Engine/Components/Skybox.h"
#include "Engine/Components/Camera.h"
#include "Engine/Components/Character.h"
#include "Engine/Components/MeshRenderer.h"
#include "Engine/Components/TerrainBezier.h"
#include "Engine/Mesh.h"
#include "Engine/MeshImporter.h"


void Manager::startloop(){
    ::Engine::Engine::instance = &engine;
    auto skyboxActor = std::make_shared<Engine::Actor>("Skybox");
    skyboxActor->addComponent(std::make_shared<Engine::Components::Skybox>());


    auto plane = std::make_shared<Engine::Actor>("Plane");
    std::shared_ptr<Engine::Components::MeshRenderer> meshRenderer = std::make_shared<Engine::Components::MeshRenderer>();
    meshRenderer->mesh = std::move(Engine::MeshImporter::loadMesh(".\\Trab5RodrigoAppelt\\assets\\models\\aviao.obj"));
    plane->addComponent(std::move(meshRenderer));
    plane->position = Vector3(0, 0, 50);

    auto plane2 = std::make_shared<Engine::Actor>("Plane2");
    std::shared_ptr<Engine::Components::MeshRenderer> meshRenderer2 = std::make_shared<Engine::Components::MeshRenderer>();
    meshRenderer2->mesh = std::move(Engine::MeshImporter::loadMesh(".\\Trab5RodrigoAppelt\\assets\\models\\aviao.obj"));
    plane2->addComponent(std::move(meshRenderer2));
    plane2->position = Vector3(0, 10, 0);
    plane2->scale = Vector3(0.5, 0.5, 0.5);
    plane->addChild(std::move(plane2));


    auto cameraActor = std::make_shared<Engine::Actor>("Camera");
    std::shared_ptr<Engine::Components::Camera> cam = std::make_shared<Engine::Components::Camera>();
    cam->isActive = true;
    cameraActor->addComponent(std::move(std::make_shared<Engine::Components::Character>()));
    cameraActor->addComponent(std::move(cam));
    cameraActor->position = Vector3(0, 0, 0);
    engine.hierarchy.push_back(std::move(cameraActor));

    auto terrain = std::make_shared<Engine::Actor>("Terrain");
    terrain->position = Vector3(-25, 0, -25);
    terrain->scale = Vector3(50, 2, 50);
    auto ter = std::make_shared<Engine::Components::TerrainBezier>();
    ter->resolution = 128;
    terrain->addComponent(std::move(ter));

    engine.hierarchy.push_back(std::move(skyboxActor));
    engine.hierarchy.push_back(std::move(terrain));
    engine.hierarchy.push_back(std::move(plane));
    engine.Start();
}

void Manager::render(){
    engine.Render();
}

void Manager::update(float delta){
    engine.Update(delta);
}

void Manager::keyboard(Key key){
    engine.KeyDown(key);
}

void Manager::keyboardUp(Key key){
    engine.KeyUp(key);
}
