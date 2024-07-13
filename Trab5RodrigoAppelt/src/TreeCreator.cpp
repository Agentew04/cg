#include "TreeCreator.h"

#include <memory>
#include <map>

#include "Engine/Components/MeshRenderer.h"
#include "Engine/MeshImporter.h"

std::shared_ptr<Engine::Actor> TreeCreator::createTree(TreeTop top){
    auto barkActor = std::make_shared<Engine::Actor>("TreeBark");
    auto topActor = std::make_shared<Engine::Actor>("TreeTop");
    
    auto barkMr = std::make_shared<Engine::Components::MeshRenderer>();
    auto topMr = std::make_shared<Engine::Components::MeshRenderer>();

    if(!barkMesh){
        barkMesh = createBark();
    }

    barkMr->mesh = barkMesh;

    // gerar mesh ou pegar uma da cache
    auto it = cachedTrees.find(top);
    if(it != cachedTrees.end()){
        auto tree = it->second;
        topMr->mesh = tree.treeTopModel;
    }else{
        // create meshes and add to cache
        Tree t;
        t.treeTopModel = createTreeTop(top);
        cachedTrees[top] = t;
        topMr->mesh = t.treeTopModel;
    }

    // add mesh renderers for each
    barkActor->addComponent(std::move(barkMr));
    topActor->addComponent(std::move(topMr));

    barkActor->addChild(std::move(topActor));
    return barkActor;
}

std::shared_ptr<Engine::Mesh> TreeCreator::createBark(){
    return Engine::MeshImporter::loadMesh(".\\Trab5RodrigoAppelt\\assets\\models\\treebark.obj");
}

std::shared_ptr<Engine::Mesh> TreeCreator::createTreeTop(TreeTop top){
    switch(top){
        case TreeTop::Sphere:
            return Engine::MeshImporter::loadMesh(".\\Trab5RodrigoAppelt\\assets\\models\\treetop_sphere.obj");
        case TreeTop::Tetrahedron:
            // runtime generation
            return nullptr;
            break;
        case TreeTop::Cube:
            return Engine::MeshImporter::loadMesh(".\\Trab5RodrigoAppelt\\assets\\models\\treetop_cube.obj");
    }
}