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
    terrainActor->scale = Vector3(200, 2, 200);
    terrainActor->position = Vector3(-100,-20,-100);
    terrain = std::make_shared<Engine::Components::TerrainBezier>();
    terrain->resolution = 128;
    terrainActor->addComponent(terrain); // sem std::move pois queremos que fique uma copia aqui
    terrainActor->Start(); // forca inicializacao para as arvores
    engine.hierarchy.push_back(std::move(terrainActor));
}

void Manager::createTrees(){
    int treeCount = 50;

    std::mt19937 rng(std::time(nullptr));

    auto terrainActor = terrain->actor.lock();
    if(!terrainActor){
        std::cout << "Nao consegui dar lock no terreno" << std::endl;
        return;
    }
    std::uniform_int_distribution<int> treeTopDist(0, (int)TreeCreator::TreeTop::COUNT - 1);
    std::uniform_real_distribution<float> positionDist(-terrainActor->scale.x*0.5f, terrainActor->scale.x*0.5f);
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
