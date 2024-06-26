#include "Manager.h"

#include "../gl_canvas2d.h"
#include "../3D/Rasterizer.h"

#define DEG_90 1.5707963267948966192313216916398

Manager::Manager(int* screenWidth, int* screenHeight) :
    scrW(screenWidth), scrH(screenHeight),
    sidebar(screenWidth, screenHeight, this),
    colorBuffer(new Buffer(*screenWidth-sidebar.getSidebarWidth(), *screenHeight, 3)),
    zBuffer(new Buffer(*screenWidth-sidebar.getSidebarWidth(), *screenHeight, 1))
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
    if(renderingMode == RenderingMode::SOLID_PIXEL){
        colorBuffer->resizeIfNeeded((*scrW-sidebar.getSidebarWidth())*renderScale, *scrH*renderScale);
        zBuffer->resizeIfNeeded((*scrW-sidebar.getSidebarWidth())*renderScale, *scrH*renderScale);
    }

    sim.update(delta);
    sidebar.update(delta);
    sim.getValues().rpm = sidebar.getRpm();
    sim.getValues().driveshaftAngle = -sidebar.getDriveshaftAngle();

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

    if(renderingMode == RenderingMode::WIREFRAME){
        drawParts();
    }else if(renderingMode == RenderingMode::SOLID_PIXEL){
        drawPixel();
    }
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

void Manager::setRenderingMode(RenderingMode renderingMode){
    this->renderingMode = renderingMode;
}

void Manager::drawParts() {
    auto piston = sim.createPiston();
    auto pistonArm = piston[0];
    auto pistonBase = piston[1];

    if(partsVisibility[SimulationPart::CRANKSHAFT]){
        auto crankshaft = sim.createCrankShaft();
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

    if(partsVisibility[SimulationPart::GEARS]){
        auto gears = sim.createGears();
        for(auto &p : gears){
            draw(p);
        }
    }

    if(partsVisibility[SimulationPart::DRIVESHAFT]){
        auto driveshaftparts = sim.createDriveShaft();
        auto driveshaftconn = sim.createDriveShaftConnector();
        for(auto &p:driveshaftparts){
            draw(p);
        }
        for(auto &p : driveshaftconn){
            draw(p);
        }
    }

}

void Manager::drawPixel() {
    // cria apenas as formas visiveis
    std::vector<Primitive> polys;
    if(partsVisibility[SimulationPart::CRANKSHAFT]){
        auto crankshaft = sim.createCrankShaft();
        polys.insert(polys.end(), crankshaft.begin(), crankshaft.end());
    }
    if(partsVisibility[SimulationPart::PISTON_BASE] || partsVisibility[SimulationPart::PISTON_ARM]){
        auto piston = sim.createPiston();
        auto pistonBase = piston[1];
        auto pistonArm = piston[0];
        if(partsVisibility[SimulationPart::PISTON_BASE]){
            polys.push_back(pistonBase);
        }
        if(partsVisibility[SimulationPart::PISTON_ARM]){
            polys.push_back(pistonArm);
        }
    }
    if(partsVisibility[SimulationPart::GEARS]){
        auto gears = sim.createGears();
        polys.insert(polys.end(), gears.begin(), gears.end());
    }
    if(partsVisibility[SimulationPart::DRIVESHAFT]){
        auto driveshaftparts = sim.createDriveShaft();
        auto driveshaftconn = sim.createDriveShaftConnector();
        polys.insert(polys.end(), driveshaftparts.begin(), driveshaftparts.end());
        polys.insert(polys.end(), driveshaftconn.begin(), driveshaftconn.end());
    }

    colorBuffer->fill(mainBackgroundColor.x, mainBackgroundColor.y, mainBackgroundColor.z);

    std::cout << "comecando raster. scale: " << renderScale << std::endl;
    Rasterizer::Rasterize(
        polys,
        cam3d,
        colorBuffer,
        zBuffer,
        Vector3(-1,-1,-1),
        renderScale
    );
    colorBuffer->display(1/renderScale);
}
