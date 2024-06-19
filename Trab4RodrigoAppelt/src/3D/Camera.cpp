#include "Camera.h"

#include "../gl_canvas2d.h"

Camera3D::Camera3D(Vector3 position, Vector3 rotation)
    : cameraPosition(position), eulerRotation(rotation) {}

Vector3 Camera3D::getPosition(){
    return cameraPosition;
}

Vector3 Camera3D::getRotation(){
    return eulerRotation;
}

Vector3 Camera3D::worldToCamera(Vector3 worldPos){
    Vector3 forward = getForward();
    Vector3 right = getRight();
    Vector3 up = getUp();

    Vector3 delta = worldPos - cameraPosition;

    float x = delta.dot(right);
    float y = delta.dot(up);
    float z = delta.dot(forward);

    return Vector3(x, y, z);
}

std::vector<Vector3> Camera3D::worldToCamera(std::vector<Vector3> worldPos){
    std::vector<Vector3> result;
    for(Vector3 pos : worldPos){
        result.push_back(worldToCamera(pos));
    }
    return result;
}

Vector3 Camera3D::getForward(){
    float x = cos(eulerRotation.y) * cos(eulerRotation.x);
    float y = sin(eulerRotation.y) * cos(eulerRotation.x);
    float z = sin(eulerRotation.x);
    return Vector3(x, y, z);
}

Vector3 Camera3D::getRight(){
    float x = cos(eulerRotation.y - 3.1415/2);
    float y = sin(eulerRotation.y - 3.1415/2);
    float z = 0;
    return Vector3(x, y, z);
}

Vector3 Camera3D::getUp(){
    return getForward().cross(getRight());
}

void Camera3D::updateMousePos(Vector2 mousePos){
    float delta = CV::delta();

    if(lastMousePos == Vector3::zero()){
        lastMousePos = Vector3(mousePos.x, mousePos.y, 0);
    }

    Vector2 mouseDelta = Vector2(mousePos.x - lastMousePos.x, mousePos.y - lastMousePos.y);

    eulerRotation.y += mouseDelta.x * delta;
    eulerRotation.x += mouseDelta.y * delta;

    // clamp x rotation
    eulerRotation.x = std::max(-3.1415f/2, std::min(3.1415f/2, eulerRotation.x));
    eulerRotation.z = 0;
    eulerRotation.y = fmod(eulerRotation.y, 2 * 3.1415f);
}

void Camera3D::keyDown(Key key){
    float delta = CV::delta();
    Vector3 forward = getForward();
    Vector3 right = getRight();
    Vector3 up = getUp();

    if(key == 'w' || key == 'W'){
        cameraPosition = cameraPosition + forward * delta;
    }
    if(key == 's' || key == 'S'){
        cameraPosition = cameraPosition - forward * delta;
    }
    if(key == 'a' || key == 'A'){
        cameraPosition = cameraPosition - right * delta;
    }
    if(key == 'd' || key == 'D'){
        cameraPosition = cameraPosition + right * delta;
    }
}

void Camera3D::calculateFrustrum(){
    if(!isFrustrumDirty){
        return;
    }

    isFrustrumDirty = false;

    Vector3 forward = getForward();
    Vector3 right = getRight();
    Vector3 up = getUp();

    Vector3 nearCenter = cameraPosition + forward * nearPlane;
    Vector3 farCenter = cameraPosition + forward * farPlane;

    Vector3 nearTopLeft = nearCenter + up * d + right * d;
    Vector3 nearTopRight = nearCenter + up * d - right * d;
    Vector3 nearBottomLeft = nearCenter - up * d + right * d;
    Vector3 nearBottomRight = nearCenter - up * d - right * d;

    Vector3 farTopLeft = farCenter + up * d + right * d;
    Vector3 farTopRight = farCenter + up * d - right * d;
    Vector3 farBottomLeft = farCenter - up * d + right * d;
    Vector3 farBottomRight = farCenter - up * d - right * d;

    frustrum.topFace = {up, nearTopLeft.dot(up)};
    frustrum.bottomFace = {-up, nearBottomLeft.dot(-up)};
    frustrum.rightFace = {right, nearTopRight.dot(right)};
    frustrum.leftFace = {-right, nearTopLeft.dot(-right)};
    frustrum.farFace = {forward, farTopLeft.dot(forward)};
    frustrum.nearFace = {-forward, nearTopLeft.dot(-forward)};
    
}

bool Camera3D::isOnFrustum(Vector3 worldPos){
    Vector3 v = worldToCamera(worldPos);

    if(v.z < nearPlane && v.z > farPlane){
        return false;
    }



    
}