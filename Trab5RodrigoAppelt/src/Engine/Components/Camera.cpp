#include "Camera.h"

Vector3 Engine::Components::Camera::direction() const{
    Vector3 dir(0,0,1);
    dir = dir.rotate(rotation.x, Vector3(1,0,0));
    dir = dir.rotate(rotation.x, Vector3(1,0,0));
    dir = dir.rotate(rotation.x, Vector3(1,0,0));
    return dir;
}
