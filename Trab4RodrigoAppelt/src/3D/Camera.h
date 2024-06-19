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
    void updateMousePos(Vector2 mousePos);
    void keyDown(Key key);
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

    float d = 200;
    float farPlane = 1000;
    float nearPlane = 0.1;
    Vector3 cameraPosition;
    Vector3 eulerRotation;
    float sensitivity = 0.01f;
    Vector3 lastMousePos = Vector3::zero();
    Frustrum frustrum;
    bool isFrustrumDirty = true;
    void calculateFrustrum();
};

#endif