#include "Manager.h"

#include <memory>
#include <random>
#include <ctime>

#include "Engine/Actor.h"
#include "Engine/Components/Skybox.h"
#include "Engine/Components/Camera.h"
#include "Engine/Components/Character.h"
#include "Engine/Components/MeshRenderer.h"
#include "Engine/Components/TerrainBezier.h"
#include "Engine/Components/Rotating.h"
#include "Engine/Mesh.h"
#include "Engine/MeshImporter.h"


void Manager::startloop(){
    // link active engine
    ::Engine::Engine::instance = &engine;
    
    createWorld();
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

void Manager::createWorld(){
    createTerrain();
    createTrees();
    createCharacter();
}

void Manager::createTerrain(){
    // skybox
    auto skyboxActor = std::make_shared<Engine::Actor>("Skybox");
    skyboxActor->addComponent(std::make_shared<Engine::Components::Skybox>());
    engine.hierarchy.push_back(std::move(skyboxActor));

    // terrain
    auto terrainActor = std::make_shared<Engine::Actor>("Terrain");
    terrainActor->scale = Vector3(1, 1, 1);
    terrainActor->position = Vector3(-0.625,10,-0.625);
    terrain = std::make_shared<Engine::Components::TerrainBezier>();
    terrain->resolution = 128;
    terrainActor->addComponent(terrain); // sem std::move pois queremos que fique uma copia aqui
    engine.hierarchy.push_back(std::move(terrainActor));

    auto cube = std::make_shared<Engine::Actor>("Cube");
    auto mesh = std::make_shared<Engine::Components::MeshRenderer>();
    mesh->mesh = std::move(Engine::MeshImporter::loadMesh(".\\Trab5RodrigoAppelt\\assets\\models\\cubo.obj"));
    cube->addComponent(std::move(mesh));
    cube->position = Vector3(0, 10, 0);
    cube->scale = Vector3(0.25,0.25,0.25);
    engine.hierarchy.push_back(std::move(cube));
}

void Manager::createTrees(){
    int treeCount = 10;

    std::mt19937 rng(std::time(nullptr));

    std::uniform_int_distribution<int> treeTopDist(0, (int)TreeCreator::TreeTop::COUNT - 1);
    std::uniform_real_distribution<float> positionDist(-25, 25);
    std::uniform_real_distribution<float> rotationDist(0,360);
    
    for(int i = 0; i < treeCount; i++){
        TreeCreator::TreeTop top = (TreeCreator::TreeTop)treeTopDist(rng);
        auto treeActor = treeCreator.createTree(top);
        
        Vector3 position = Vector3(positionDist(rng), 0, positionDist(rng));
        float y = terrain->getHeightAt(position.x, position.z);
        position.y = y;
        treeActor->scale = Vector3::one();
        treeActor->position = position;
        treeActor->rotation = Vector3(0, rotationDist(rng), 0);
        std::cout << "create tree with top: " << (int)top << "; pos: " << position << "; rotation: " << treeActor->rotation << std::endl;

        engine.hierarchy.push_back(std::move(treeActor));
    }
}

void Manager::createCharacter(){
    player = std::make_shared<Engine::Components::Character>();
    auto cam = std::make_shared<Engine::Components::Camera>();
    cam->isActive = true;

    auto cameraActor = std::make_shared<Engine::Actor>("Camera");
    cameraActor->addComponent(std::move(cam));
    cameraActor->addComponent(player); // tbm sem std::move

    engine.hierarchy.push_back(std::move(cameraActor));
}
