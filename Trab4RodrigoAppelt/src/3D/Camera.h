#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <vector>

#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#include "../Keyboard.h"

class Camera3D {
public:

    Camera3D(Vector3 position = Vector3::zero(), Vector3 rotation = Vector3::zero());

    Vector3 getPosition();
    Vector3 getRotation();

    Vector3 worldToCamera(Vector3 worldPos);
    std::vector<Vector3> worldToCamera(std::vector<Vector3> worldPos);

    bool isOnFrustum(Vector3 worldPos);

    Vector3 getForward();
    Vector3 getRight();
    Vector3 getUp();

    // event forwarding
    void keyDown(Key key);
    void keyUp(Key key);
    void update(float delta);
private:

    // frustrum culling pego de: https://learnopengl.com/Guest-Articles/2021/Scene/Frustum-Culling
    struct Plane {
        Vector3 normal;
        float distance;
    };

    struct Frustrum {
        Plane topFace;
        Plane bottomFace;

        Plane rightFace;
        Plane leftFace;

        Plane farFace;
        Plane nearFace;
    };

    enum class Movement {
        NONE = 0,
        MOVE_FORWARD = 1 << 0,
        MOVE_BACKWARD = 1 << 1,
        MOVE_LEFT = 1 << 2,
        MOVE_RIGHT = 1 << 3,
        MOVE_UP = 1 << 4,
        MOVE_DOWN = 1 << 5,
        LOOK_LEFT = 1 << 6,
        LOOK_RIGHT = 1 << 7,
        LOOK_UP = 1 << 8,
        LOOK_DOWN = 1 << 9,
    };

    float d = 200;
    float farPlane = 1000;
    float nearPlane = 0.1;
    float moveSpeed = 50;
    Vector3 cameraPosition;
    Vector3 eulerRotation;
    float sensitivity = 2.0f;
    Vector3 lastMousePos = Vector3::zero();
    Movement movement = Movement::NONE;
    Frustrum frustrum;
    bool isFrustrumDirty = true;
    void calculateFrustrum();
};

#endif