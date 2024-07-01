#ifndef __MANAGER_H__
#define __MANAGER_H__

#include "../Math/Vector2.h"
#include "../Keyboard.h"

#include "Simulator.h"
#include "Sidebar.h"
#include "../3D/Camera.h"
#include "../3D/Buffer.h"

/// @brief Classe que junta todos os outros componentes da aplicacao.
/// Cuida do estado e gerencia tudo.
class Manager {
public:

    /// @brief Controla qual camera esta ativa para o 3D
    enum class CameraMode {
        PERSPECTIVE_FREE,
        ORTHO_SIDE,
        ORTHO_FRONT,
        ORTHO_TOP
    };

    /// @brief Controla qual pecas da simulacao sao calculadas e visiveis
    enum class SimulationPart {
        CRANKSHAFT,
        PISTON_BASE,
        PISTON_ARM,
        GEARS,
        DRIVESHAFT,
        COUNT
    };

    /// @brief Controla qual engine de renderizacao usada
    enum class RenderingMode {
        WIREFRAME,
        SOLID_PIXEL,
        // talvez solid vertex aqui tbm
    };

    /// @brief Representa um dos buffers da tela
    enum class DisplayBuffer {
        COLOR,
        DEPTH,
        NORMALS
        // se tivesse uso, o color e depth stencil iria aqui
    };

    Manager(int* screenWidth, int* screenHeight);

    // functions
    void setCameraMode(CameraMode cameraMode);
    void setVisibility(SimulationPart part, bool visibility);
    void setRenderingMode(RenderingMode renderingMode);
    void setDisplayBuffer(DisplayBuffer displayBuffer);

    // event forwarding
    void keyDown(Key key);
    void keyUp(Key key);
    void update(float delta);
    void render();
    void updateMousePos(Vector2 mousePos);
    void mouseDown();
    void mouseUp();
private:
    // tela
    int *scrW, *scrH;

    // estilo
    Vector3 mainBackgroundColor = Vector3::fromHex(0x8ecae6);

    // estado geral
    Simulator sim;
    Sidebar sidebar;
    Camera3D cam3d;
    Camera2D cam2d_front;
    Camera2D cam2d_side;
    Camera2D cam2d_top;

    // estado especifico
    CameraMode cameraMode = CameraMode::PERSPECTIVE_FREE;
    std::map<SimulationPart, bool> partsVisibility;

    // pixel rendering
    RenderingMode renderingMode = RenderingMode::WIREFRAME;
    DisplayBuffer displayBuffer = DisplayBuffer::COLOR;
    Buffer *colorBuffer = nullptr;
    Buffer *zBuffer = nullptr;
    float renderScale = 1;
    float ambientLightIntensity = 0.2f;

    // utilitarias
    void drawParts();
    void drawPixel();
    void draw(const Primitive& p) const;
};

#endif