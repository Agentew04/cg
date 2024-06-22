#include "Manager.h"

#include "../gl_canvas2d.h"

#define DEG_90 1.5707963267948966192313216916398

Manager::Manager(int* screenWidth, int* screenHeight) :
    scrW(screenWidth), scrH(screenHeight),
    sidebar(screenWidth, screenHeight, this)
{
    // cria cameras
    cam3d.setD(640); // +- 90 de fov para 720p
    cam3d.setPosition(Vector3(-300,0, 50));

    cam2d_front.setPosition(Vector3(-300,0, 50));
    cam2d_front.setRotation(Vector3(0, 0, 0));

    cam2d_side.setPosition(Vector3(0, -300, 50));
    cam2d_side.setRotation(Vector3(0, DEG_90, 0));

    cam2d_top.setPosition(Vector3(0,0, 300));
    cam2d_top.setRotation(Vector3(DEG_90, 0, 0));

    for(int i=0;i<(int)SimulationPart::COUNT;i++){
        partsVisibility[(SimulationPart)i] = true;
    }
}

#pragma region Events

void Manager::keyDown(Key key) {
    switch(cameraMode){
        case CameraMode::PERSPECTIVE_FREE:
            cam3d.keyDown(key);
            break;
        case CameraMode::ORTHO_FRONT:
            cam2d_front.keyDown(key);
            break;
        case CameraMode::ORTHO_SIDE:
            cam2d_side.keyDown(key);
            break;
        case CameraMode::ORTHO_TOP:
            cam2d_top.keyDown(key);
            break;
    }
}

void Manager::keyUp(Key key) {
    switch(cameraMode){
        case CameraMode::PERSPECTIVE_FREE:
            cam3d.keyUp(key);
            break;
        case CameraMode::ORTHO_FRONT:
            cam2d_front.keyUp(key);
            break;
        case CameraMode::ORTHO_SIDE:
            cam2d_side.keyUp(key);
            break;
        case CameraMode::ORTHO_TOP:
            cam2d_top.keyUp(key);
            break;
    }
}

void Manager::update(float delta) {
    sim.update(delta);
    sidebar.update(delta);
    sim.getValues().rpm = sidebar.getRpm();

    switch(cameraMode){
        case CameraMode::PERSPECTIVE_FREE:
            cam3d.update(delta);
            break;
        case CameraMode::ORTHO_FRONT:
            cam2d_front.update(delta);
            break;
        case CameraMode::ORTHO_SIDE:
            cam2d_side.update(delta);
            break;
        case CameraMode::ORTHO_TOP:
            cam2d_top.update(delta);
            break;
    }
}

void Manager::render() {
    CV::clear(mainBackgroundColor);
    CV::translate(*scrW/2, *scrH/2);

    

    drawParts();

    sidebar.render();
}

void Manager::updateMousePos(Vector2 mousePos) {
    sidebar.updateMousePos(mousePos);
}

void Manager::mouseDown() {
    sidebar.mouseDown();
}

void Manager::mouseUp() {
    sidebar.mouseUp();
}

#pragma endregion

void Manager::draw(const Primitive& p) const {
    switch(cameraMode){
        case CameraMode::PERSPECTIVE_FREE:
            cam3d.draw(p);
            break;
        case CameraMode::ORTHO_FRONT:
            cam2d_front.draw(p);
            break;
        case CameraMode::ORTHO_SIDE:
            cam2d_side.draw(p);
            break;
        case CameraMode::ORTHO_TOP:
            cam2d_top.draw(p);
            break;
    }
}

void Manager::setCameraMode(CameraMode cameraMode){
    this->cameraMode = cameraMode;
}

void Manager::setVisibility(SimulationPart part, bool visibility){
    partsVisibility[part] = visibility;
}

void Manager::drawParts() {
    auto crankshaft = sim.createCrankShaft(sim.getValues());
    auto piston = sim.createPiston(sim.getValues());
    auto pistonArm = piston[0];
    auto pistonBase = piston[1];

    if(partsVisibility[SimulationPart::CRANKSHAFT]){
        for(auto &p : crankshaft){
            draw(p);
        }
    }

    if(partsVisibility[SimulationPart::PISTON_BASE]){
        draw(pistonBase);
    }

    if(partsVisibility[SimulationPart::PISTON_ARM]){
        draw(pistonArm);
    }
}
