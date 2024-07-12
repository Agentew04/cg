#include "Actor.h"

#include <GL/glut.h>

using namespace Engine;

void Actor::Start(){
    for (auto& component : components){
        component->Start();
    }
    for (auto& child : children){
        child->Start();
    }
}

void Actor::Update(float delta){
    for (auto& component : components){
        component->Update(delta);
    }
    for (auto& child : children){
        child->Update(delta);
    }
}

void Actor::Destroy(){
    for (auto& child : children){
        child->Destroy();
    }
    for (auto& component : components){
        component->Destroy();
    }

    components.clear();
    children.clear();
}

void Actor::Render(){
    glPushMatrix();

    glTranslatef(position.x, position.y, position.z);
    glScalef(scale.x, scale.y, scale.z);
    glRotatef(rotation.x, 1, 0, 0);
    glRotatef(rotation.y, 0, 1, 0);
    glRotatef(rotation.z, 0, 0, 1);
    
    std::cout << "Renderizando " << name << std::endl;
    for (auto& component : components){
        component->Render();
    }
    for (auto& child : children){
        child->Render();
    }

    glPopMatrix();
}

void Actor::KeyDown(Key key){
    for (auto& component : components){
        component->KeyDown(key);
    }
    for (auto& child : children){
        child->KeyDown(key);
    }
}

void Actor::KeyUp(Key key){
    for (auto& component : components){
        component->KeyUp(key);
    }
    for (auto& child : children){
        child->KeyUp(key);
    }
}

Vector3 Actor::getForward() const {
    auto fwd = Vector3(0,0,1);
    if(auto prnt = parent.lock()){
        fwd = prnt->getForward();
    }
    fwd = fwd.rotate(rotation.x, Vector3(1,0,0));
    fwd = fwd.rotate(rotation.y, Vector3(0,1,0));
    fwd = fwd.rotate(rotation.z, Vector3(0,0,1));
    return fwd;
}

void Actor::addChild(std::shared_ptr<Actor> child) {
    child->parent = shared_from_this();
    children.push_back(child);
}
