#include "TreeCreator.h"

#include <memory>
#include <map>

#include "Engine/Components/MeshRenderer.h"
#include "Engine/MeshImporter.h"
#include "Engine/Mesh.h"

std::shared_ptr<Engine::Actor> TreeCreator::createTree(TreeTop top){
    auto barkActor = std::make_shared<Engine::Actor>("TreeBark");
    auto topActor = std::make_shared<Engine::Actor>("TreeTop");
    topActor->position = Vector3(0, 10, 0);
    topActor->scale = Vector3(3,3,3);

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
    std::shared_ptr<Engine::Mesh> mesh;
    
    switch(top){
        case TreeTop::Sphere:
            mesh = Engine::MeshImporter::loadMesh(".\\Trab5RodrigoAppelt\\assets\\models\\treetop_sphere.obj");
            break;
        case TreeTop::Tetrahedron: {
                // runtime generation
                auto tetra = Engine::Mesh::tetrahedron(4);
                Engine::Material treeTopMat = Engine::Material();
                treeTopMat.setDiffuse(0.0, 0.8, 0.0, 1.0);
                treeTopMat.setAmbient(0.1, 0.4, 0.1, 1.0);
                treeTopMat.shininess = 10;
                treeTopMat.setSpecular(1,1,1,1);
                tetra.materials.push_back(treeTopMat);
                mesh = std::make_shared<Engine::Mesh>(tetra);
            }
            break;
        case TreeTop::Cube:
            mesh = Engine::MeshImporter::loadMesh(".\\Trab5RodrigoAppelt\\assets\\models\\treetop_cube.obj");
            break;
        default:
            mesh = nullptr;
    }
    
    return mesh;
}
