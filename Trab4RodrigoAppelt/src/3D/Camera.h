#ifndef __CAMERA_H__
#define __CAMERA_H__

#include <vector>

#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#include "../Keyboard.h"

/// @brief Camera em perspectiva para campos 3d.
class Camera3D {
public:

    /// @brief Cria uma camera 3D
    /// @param position A posicao dela no mundo
    /// @param rotation A rotacao dela. Os angulos sao em graus.
    /// @param d O parametro 'd'. 640 equivale a +- fov=90 graus com uma tela 1280x720.
    /// @param farPlane O maximo da coordenada Z que arestas sao renderizadas
    /// @param nearPlane O minimo da coordenada Z que arestas sao renderizadas
    Camera3D(
        Vector3 position = Vector3::zero(),
        Vector3 rotation = Vector3::zero(),
        float d = 200,
        float farPlane = 1000,
        float nearPlane = 0.1);

    /// @brief Retorna a posicao da camera no mundo
    Vector3 getPosition() const;
    /// @brief Retorna a rotacao da camera no mundo. Os valores sao em graus.
    Vector3 getRotation() const;

    /// @brief Define a nova posicao da camera no mundo
    /// @param A nova posicao da camera
    void setPosition(Vector3 position);
    /// @brief Define a nova rotacao da camera
    /// @param eulerRotation A rotacao. Os valores sao em graus.
    void setRotation(Vector3 eulerRotation);

    /// @brief Converte um vertice de coordenadas do mundo para coordenadas de visualizacao.
    /// @param worldPos As coordenadas de mundo a serem convertidas
    /// @return As coordenadas no espaco de visualizacao
    Vector3 worldToCamera(Vector3 worldPos) const;
    /// @brief Converte um vetor de coordenadas do mundo para visualizacao
    /// @param worldPos Um vetor com as coord do mundo
    /// @return Um vetor com as coords de visualizacao
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

    /// @brief Retorna o vetor para frente da camera. Eh aonde ela esta apontando
    Vector3 getForward() const;
    /// @brief Retorna o vetor para a direita da camera.
    Vector3 getRight() const;
    /// @brief Retorna o vetor para cima da camera. Eh (forward x right)
    Vector3 getUp() const;

    /// @brief Retorna o parametro 'd' atual
    float getD() const;
    /// @brief Define o parametro 'd' a ser usado na camera. 640 ~= fov 90 graus em 1280x720
    void setD(float d);

    /// @brief Retorna o fov da camera.
    /// @param screenSize O tamanho do display da camera
    /// @return Um vetor com o fov horizontal e vertical
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
    Movement movement = Movement::NONE;
};

/// @brief Classe que representa uma camera ortografica
/// para uso em aplicacoes 2D.
class Camera2D {
public:
    /// @brief Cria uma camera 2D ortografica
    /// @param position A posicao da camera no mundo 2d
    /// @param rotation 
    Camera2D(
        Vector3 position = Vector3::zero(),
        Vector3 rotation = Vector3::zero());

    Vector3 getPosition() const;
    Vector3 getRotation() const;

    void setPosition(Vector3 position);
    void setRotation(Vector3 eulerRotation);

    Vector3 worldToCamera(Vector3 worldPos) const;
    std::vector<Vector3> worldToCamera(const std::vector<Vector3>& worldPos) const;

    bool isOnFrustum(Vector3 pos) const;

    Vector3 getForward() const;
    Vector3 getRight() const;
    Vector3 getUp() const;


    // event forwarding
    void keyDown(Key key);
    void keyUp(Key key);
    void update(float delta);
private:

    enum class Movement {
        NONE = 0,
        MOVE_LEFT = 1 << 0,
        MOVE_RIGHT = 1 << 1,
        MOVE_UP = 1 << 2,
        MOVE_DOWN = 1 << 3
    };

    float moveSpeed = 20;
    Vector3 cameraPosition;
    Vector3 eulerRotation;
    Movement movement = Movement::NONE;
};

#endif
