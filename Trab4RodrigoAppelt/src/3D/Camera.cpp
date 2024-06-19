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
    Vector3 right = getRight();
    Vector3 up = getUp();

    std::cout << "rotation: " << eulerRotation << std::endl;
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
    std::cout << std::endl;
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

    calculateFrustrum();

    if(frustrum.topFace.normal.dot(v) + frustrum.topFace.distance < 0){
        return false;
    }

    if(frustrum.bottomFace.normal.dot(v) + frustrum.bottomFace.distance < 0){
        return false;
    }

    if(frustrum.rightFace.normal.dot(v) + frustrum.rightFace.distance < 0){
        return false;
    }

    if(frustrum.leftFace.normal.dot(v) + frustrum.leftFace.distance < 0){
        return false;
    }

    if(frustrum.farFace.normal.dot(v) + frustrum.farFace.distance < 0){
        return false;
    }

    if(frustrum.nearFace.normal.dot(v) + frustrum.nearFace.distance < 0){
        return false;
    }

    return true;
}