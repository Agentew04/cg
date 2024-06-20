#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <vector>

#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#include "../Keyboard.h"

class Camera3D {
public:

    Camera3D(
        Vector3 position = Vector3::zero(),
        Vector3 rotation = Vector3::zero(),
        float d = 200,
        float farPlane = 1000,
        float nearPlane = 0.1);

    Vector3 getPosition() const;
    Vector3 getRotation() const;

    Vector3 worldToCamera(Vector3 worldPos) const;
    std::vector<Vector3> worldToCamera(std::vector<Vector3> worldPos) const;

    /// @brief Verifica se uma posicao em espaco relativo de mundo esta dentro do frustum. Por enquanto nao considera os planos laterais.
    /// Apenas o far e near plane.
    /// @param worldSpacePos A posicao
    /// @return true se esta dentro do frustum, false caso contrario
    bool isOnFrustumWS(Vector3 worldSpacePos) const;
    /// @brief Verifica se uma posicao em espaco relativo de camera esta dentro do frustum. Por enquanto nao considera os planos laterais.
    /// @param cameraSpacePos A posicao
    /// @return true se esta dentro do frustum, false caso contrario
    bool isOnFrustumCS(Vector3 cameraSpacePos) const;

    Vector3 getForward() const;
    Vector3 getRight() const;
    Vector3 getUp() const;

    float getD() const;
    void setD(float d);

    Vector2 getFov(Vector2 screenSize) const;

    // event forwarding
    void keyDown(Key key);
    void keyUp(Key key);
    void update(float delta);
private:

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
    float moveSpeed = 20;
    Vector3 cameraPosition;
    Vector3 eulerRotation;
    float sensitivity = 2.0f;
    Vector3 lastMousePos = Vector3::zero();
    Movement movement = Movement::NONE;
};

#endif
