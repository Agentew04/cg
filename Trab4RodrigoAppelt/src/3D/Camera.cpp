#include "Camera.h"

#include "../gl_canvas2d.h"

Camera3D::Camera3D(
    Vector3 position, 
    Vector3 rotation,
    float d,
    float farPlane,
    float nearPlane)
    : cameraPosition(position), 
    eulerRotation(rotation),
    d(d),
    farPlane(farPlane),
    nearPlane(nearPlane) {}

Vector3 Camera3D::getPosition() const {
    return cameraPosition;
}

Vector3 Camera3D::getRotation() const{
    return eulerRotation;
}

Vector3 Camera3D::worldToCamera(Vector3 worldPos) const{
    Vector3 forward = getForward();
    Vector3 right = getRight();
    Vector3 up = getUp();

    Vector3 delta = worldPos - cameraPosition;

    float x = delta.dot(right);
    float y = delta.dot(up);
    float z = delta.dot(forward);

    return Vector3(x, y, z);
}

std::vector<Vector3> Camera3D::worldToCamera(std::vector<Vector3> worldPos) const{
    std::vector<Vector3> result;
    for(Vector3 pos : worldPos){
        result.push_back(worldToCamera(pos));
    }
    return result;
}

Vector3 Camera3D::getForward() const{
    float x = cos(eulerRotation.y) * cos(eulerRotation.x);
    float y = sin(eulerRotation.y) * cos(eulerRotation.x);
    float z = sin(eulerRotation.x);
    return Vector3(x, y, z);
}

Vector3 Camera3D::getRight() const{
    float x = cos(eulerRotation.y - 3.1415/2);
    float y = sin(eulerRotation.y - 3.1415/2);
    float z = 0;
    return Vector3(x, y, z);
}

Vector3 Camera3D::getUp() const{
    return getForward().cross(getRight());
}



void Camera3D::keyDown(Key key){
    switch(key){
        case 'w':
        case 'W':
            movement = (Movement)((int)movement | (int)Movement::MOVE_FORWARD);
            break;
        case 's':
        case 'S':
            movement = (Movement)((int)movement | (int)Movement::MOVE_BACKWARD);
            break;
        case 'a':
        case 'A':
            movement = (Movement)((int)movement | (int)Movement::MOVE_LEFT);
            break;
        case 'd':
        case 'D':
            movement = (Movement)((int)movement | (int)Movement::MOVE_RIGHT);
            break;
        case 'e':
        case 'E':
        case SPACE:
            movement = (Movement)((int)movement | (int)Movement::MOVE_UP);
            break;
        case 'q':
        case 'Q':
        case 'c':
        case 'C':
        case LEFT_SHIFT:
            movement = (Movement)((int)movement | (int)Movement::MOVE_DOWN);
            break;
        case LEFT_ARROW:
            movement = (Movement)((int)movement | (int)Movement::LOOK_LEFT);
            break;
        case RIGHT_ARROW:
            movement = (Movement)((int)movement | (int)Movement::LOOK_RIGHT);
            break;
        case UP_ARROW:
            movement = (Movement)((int)movement | (int)Movement::LOOK_UP);
            break;
        case DOWN_ARROW:
            movement = (Movement)((int)movement | (int)Movement::LOOK_DOWN);
            break;
    }
}

void Camera3D::keyUp(Key key){
    switch(key){
        case 'w':
        case 'W':
            movement = (Movement)((int)movement & ~(int)Movement::MOVE_FORWARD);
            break;
        case 's':
        case 'S':
            movement = (Movement)((int)movement & ~(int)Movement::MOVE_BACKWARD);
            break;
        case 'a':
        case 'A':
            movement = (Movement)((int)movement & ~(int)Movement::MOVE_LEFT);
            break;
        case 'd':
        case 'D':
            movement = (Movement)((int)movement & ~(int)Movement::MOVE_RIGHT);
            break;
        case 'e':
        case 'E':
        case SPACE:
            movement = (Movement)((int)movement & ~(int)Movement::MOVE_UP);
            break;
        case 'q':
        case 'Q':
        case 'c':
        case 'C':
        case LEFT_SHIFT:
            movement = (Movement)((int)movement & ~(int)Movement::MOVE_DOWN);
            break;
        case LEFT_ARROW:
            movement = (Movement)((int)movement & ~(int)Movement::LOOK_LEFT);
            break;
        case RIGHT_ARROW:
            movement = (Movement)((int)movement & ~(int)Movement::LOOK_RIGHT);
            break;
        case UP_ARROW:
            movement = (Movement)((int)movement & ~(int)Movement::LOOK_UP);
            break;
        case DOWN_ARROW:
            movement = (Movement)((int)movement & ~(int)Movement::LOOK_DOWN);
            break;
    }
}


void Camera3D::update(float delta){
    // atualizar o movimento da cam
    Vector3 forward = getForward();
    // projetar forward no plano XY
    forward.z = 0;
    forward = forward.normalized();
    Vector3 right = getRight();
    Vector3 up = getUp();

    if((int)movement & (int)Movement::MOVE_FORWARD){
        cameraPosition += forward * moveSpeed * delta;
    }
    if((int)movement & (int)Movement::MOVE_BACKWARD){
        cameraPosition -= forward * moveSpeed * delta;
    }
    if((int)movement & (int)Movement::MOVE_LEFT){
        cameraPosition -= right * moveSpeed * delta;
    }
    if((int)movement & (int)Movement::MOVE_RIGHT){
        cameraPosition += right * moveSpeed * delta;
    }
    if((int)movement & (int)Movement::MOVE_UP){
        cameraPosition += Vector3(0,0,1) * moveSpeed * delta; // up e down invertidos pq Y cresce pra baixo
    }
    if((int)movement & (int)Movement::MOVE_DOWN){
        cameraPosition -= Vector3(0,0,1) * moveSpeed * delta; // n usamos o up da camera pq n queremos voar
                                                             // para frente quando olhamos p baixo
    }

    if((int)movement & (int)Movement::LOOK_LEFT){
        eulerRotation.y += delta * sensitivity;
    }
    if((int)movement & (int)Movement::LOOK_RIGHT){
        eulerRotation.y -= delta * sensitivity;
    }
    if((int)movement & (int)Movement::LOOK_UP){
        eulerRotation.x += delta * sensitivity;
    }
    if((int)movement & (int)Movement::LOOK_DOWN){
        eulerRotation.x -= delta * sensitivity;
    }
    eulerRotation.z = 0;
    eulerRotation.x = std::max(-3.1415f/2, std::min(3.1415f/2, eulerRotation.x));
    eulerRotation.y = fmod(eulerRotation.y, 3.1415f * 2);
}

bool Camera3D::isOnFrustumWS(Vector3 worldSpacePos) const{
    Vector3 v = worldToCamera(worldSpacePos);

    if(v.z < nearPlane || v.z > farPlane){
        return false;
    }
    return true;
}

bool Camera3D::isOnFrustumCS(Vector3 cameraSpacePos) const{
    if(cameraSpacePos.z < nearPlane || cameraSpacePos.z > farPlane){
        return false;
    }
    return true;
}

float Camera3D::getD() const{
    return d;
}

void Camera3D::setD(float d){
    this->d = d;
}

Vector2 Camera3D::getFov(Vector2 screenSize) const{
    float horizontalFov = 2 * atan(screenSize.x / (2 * d));
    float verticalFov = 2 * atan(screenSize.y / (2 * d));
    return Vector2(horizontalFov, verticalFov);
}