#include "Camera.h"

#include "../gl_canvas2d.h"
#include "Primitive.h"
#include "Perspective.h"

Camera2D::Camera2D(Vector3 position, Vector3 rotation) :
    cameraPosition(position),
    eulerRotation(rotation){}

Vector3 Camera2D::getPosition() const{
    return cameraPosition;
}

void Camera2D::setPosition(Vector3 position){
    this->cameraPosition = position;
}

Vector3 Camera2D::getRotation() const{
    return eulerRotation;
}

void Camera2D::setRotation(Vector3 rotation){
    this->eulerRotation = rotation;
}

Vector3 Camera2D::worldToCamera(Vector3 worldPos) const {
    Vector3 forward = getForward();
    Vector3 right = getRight();
    Vector3 up = getUp();

    Vector3 delta = worldPos - cameraPosition;

    float x = delta.dot(right);
    float y = delta.dot(up);
    float z = delta.dot(forward);

    return Vector3(x, y, z);
}

std::vector<Vector3> Camera2D::worldToCamera(const std::vector<Vector3>& worldPos) const {
    std::vector<Vector3> arr(worldPos.size());
    for (size_t i = 0; i < worldPos.size(); i++)
    {
        arr[i] = worldToCamera(worldPos[i]);
    }
    return arr;
}

bool Camera2D::isOnFrustum(Vector3 pos) const {
    bool isOn = pos.z > cameraPosition.z;
    // if(!isOn){
    //     std::cout << "off" << std::endl;
    // }
    return true;//isOn;
}

Vector3 Camera2D::getForward() const{
    float x = cos(eulerRotation.y) * cos(eulerRotation.x);
    float y = sin(eulerRotation.y) * cos(eulerRotation.x);
    float z = sin(eulerRotation.x);
    return Vector3(x, y, z);
}
Vector3 Camera2D::getRight() const{
    float x = cos(eulerRotation.y - 3.141592/2);
    float y = sin(eulerRotation.y - 3.141592/2);
    float z = 0;
    return Vector3(x, y, z);
}
Vector3 Camera2D::getUp() const{
    return getForward().cross(getRight());
}

void Camera2D::keyDown(Key key){
    switch(key){
        case 'w':
        case 'W':
        case 'e':
        case 'E':
        case SPACE:
        case UP_ARROW:
            movement = (Movement)((int)movement | (int)Movement::MOVE_UP);
            break;
        case 'q':
        case 'Q':
        case 'c':
        case 'C':
        case LEFT_SHIFT:
        case 's':
        case 'S':
        case DOWN_ARROW:
            movement = (Movement)((int)movement | (int)Movement::MOVE_DOWN);
            break;
        case 'a':
        case 'A':
        case LEFT_ARROW:
            movement = (Movement)((int)movement | (int)Movement::MOVE_LEFT);
            break;
        case 'd':
        case 'D':
        case RIGHT_ARROW:
            movement = (Movement)((int)movement | (int)Movement::MOVE_RIGHT);
            break;
    }
}

void Camera2D::keyUp(Key key){
    switch(key){
        case 'w':
        case 'W':
        case 'e':
        case 'E':
        case SPACE:
        case UP_ARROW:
            movement = (Movement)((int)movement & ~(int)Movement::MOVE_UP);
            break;
        case 'q':
        case 'Q':
        case 'c':
        case 'C':
        case LEFT_SHIFT:
        case 's':
        case 'S':
        case DOWN_ARROW:
            movement = (Movement)((int)movement & ~(int)Movement::MOVE_DOWN);
            break;
        case 'a':
        case 'A':
        case LEFT_ARROW:
            movement = (Movement)((int)movement & ~(int)Movement::MOVE_LEFT);
            break;
        case 'd':
        case 'D':
        case RIGHT_ARROW:
            movement = (Movement)((int)movement & ~(int)Movement::MOVE_RIGHT);
            break;
    }
}


void Camera2D::update(float delta){
    // atualizar o movimento da cam
    Vector3 forward = getForward();
    // projetar forward no plano XY
    forward.z = 0;
    forward = forward.normalized();
    Vector3 right = getRight();
    Vector3 up = getUp();

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
    eulerRotation.z = 0;
    eulerRotation.x = std::max(-3.1415f/2, std::min(3.1415f/2, eulerRotation.x));
    eulerRotation.y = fmod(eulerRotation.y, 3.1415f * 2);
}

void Camera2D::draw(const Primitive& p) const{
    auto vc = worldToCamera(p.vertexList);
    CV::color(p.color);
    for(auto edge : p.edgeList){
        Vector3 v3_1 = vc[edge[0]];
        Vector3 v3_2 = vc[edge[1]];
        Vector2 v1 = Vector2(v3_1.x, v3_1.y);
        Vector2 v2 = Vector2(v3_2.x, v3_2.y);
        if(isOnFrustum(vc[edge[0]]) && isOnFrustum(vc[edge[1]])){
            CV::line(v1, v2);
        }
    }
}