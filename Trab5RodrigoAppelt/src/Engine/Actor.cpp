#include "Actor.h"

#include <GL/glut.h>

using namespace Engine;

void Actor::Start(){
    for (auto& component : components){
        component->Start();
    }
}

void Actor::Update(float delta){
    for (auto& component : components){
        component->Update(delta);
    }
    for (auto& child : children){
        child.Update(delta);
    }
}

void Actor::Destroy(){
    for (auto& child : children){
        child.Destroy();
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
    glRotatef(rotation.x, 1, 0, 0);
    glRotatef(rotation.y, 0, 1, 0);
    glRotatef(rotation.z, 0, 0, 1);

    for (auto& component : components){
        component->Render();
    }
    for (auto& child : children){
        child.Render();
    }

    glPopMatrix();
}

void Actor::KeyDown(Key key){
    for (auto& component : components){
        component->KeyDown(key);
    }
    for (auto& child : children){
        child.KeyDown(key);
    }
}

void Actor::KeyUp(Key key){
    for (auto& component : components){
        component->KeyUp(key);
    }
    for (auto& child : children){
        child.KeyUp(key);
    }
}