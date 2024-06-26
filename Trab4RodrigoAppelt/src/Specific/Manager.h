#ifndef __MANAGER_H__
#define __MANAGER_H__

#include "../Math/Vector2.h"
#include "../Keyboard.h"

#include "Simulator.h"
#include "Sidebar.h"
#include "../3D/Camera.h"

/// @brief Classe que junta todos os outros componentes da aplicacao.
/// Cuida do estado e gerencia tudo.
class Manager {
public:

    enum class CameraMode {
        PERSPECTIVE_FREE,
        ORTHO_SIDE,
        ORTHO_FRONT,
        ORTHO_TOP
    };

    enum class SimulationPart {
        CRANKSHAFT,
        PISTON_BASE,
        PISTON_ARM,
        GEARS,
        DRIVESHAFT,
        COUNT
    };

    Manager(int* screenWidth, int* screenHeight);

    // functions
    void setCameraMode(CameraMode cameraMode);
    void setVisibility(SimulationPart part, bool visibility);

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

    // utilitarias
    void drawParts();
    void draw(const Primitive& p) const;
};

#endif