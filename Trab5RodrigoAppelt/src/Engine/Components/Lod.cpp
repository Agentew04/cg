#include "Lod.h"

#include "../Engine.h"
#include "Camera.h"

using namespace Engine::Components;

void Lod::Update(float delta)
{
    // find first active camera
    std::shared_ptr<Camera> camera;
    for(auto& cam: Engine::instance->GetAllComponentsOfType<Camera>()){
        if(cam->isActive){
            camera = cam;
            break;
        }
    }

    if(!camera){
        log(LogLevel::ERROR, "No active camera found");
        return;
    }
    auto camActor = camera->actor.lock();
    auto actorHandle = this->actor.lock();
    if(!camActor || !actorHandle){
        log(LogLevel::ERROR, "Camera or actor not found. Could not lock.");
        return;
    }

    auto distance = (camActor->getGlobalPosition() - actorHandle->getGlobalPosition()).magnitude;
    auto mesh = meshes[0];

    for (int i = 0; i < levels; i++)
    {
        if (distance < distances[i])
        {
            mesh = meshes[i];
            break;
        }
    }
    if(distance > distances[levels-1]){
        mesh = meshes[levels];
    }

    meshRenderer->mesh = mesh;
}

void Lod::Start()
{
    auto act = actor.lock();
    if(!act){
        log(LogLevel::ERROR, "Actor not found. Could not lock.");
        return;
    }
    if(!act->HasComponent<MeshRenderer>()){
        log(LogLevel::ERROR, "Actor does not have a MeshRenderer component");
        return;
    }
    meshRenderer = act->GetComponent<MeshRenderer>();
    if (!meshRenderer)
    {
        log(LogLevel::ERROR, "Lod component requires a MeshRenderer component");
        return;
    }
    if(meshes.size() <= 0){
        log(LogLevel::ERROR, "Lod component requires at least one mesh");
        return;
    }
    if(meshes.size() != distances.size()+1){
        log(LogLevel::ERROR, "Number of meshes must be one more than number of distances");
        return;
    }
    levels = distances.size();
}