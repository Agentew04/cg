#include "SideBar.h"

#include "../gl_canvas2d.h"
#include "../Math/Vector2.h"
#include "Manager.h"

Sidebar::Sidebar(int* screenWidth, int* screenHeight, Manager* manager)
    : scrW(screenWidth), scrH(screenHeight), manager(manager) {
    submitUI();
}

#pragma region Events

void Sidebar::update(float delta) {

}

void Sidebar::render() {
    CV::translate(*scrW-sidebarWidth, 0);
    CV::color(sidebarColor);
    CV::rectFill(Vector2::zero(), Vector2(sidebarWidth, *scrH));

    // rpm controls
    CV::color(Vector3::fromHex(0x000000));
    CV::text(Vector2(margin,margin), "RPM:", 25, FontName::JetBrainsMono, UIPlacement::TOP_LEFT);
    sliderManager.draw();

    // camera controls
    CV::translate(*scrW-sidebarWidth, 25+30+margin*2);
    CV::color(Vector3::fromHex(0x000000));
    CV::text(Vector2(margin,margin), "Camera", 25, FontName::JetBrainsMono, UIPlacement::TOP_LEFT);
    buttonManager.draw();
}

void Sidebar::updateMousePos(Vector2 mousePos) {
    this->mousePos = mousePos;
    sliderManager.updateMousePos(mousePos);
    buttonManager.updateMousePos(mousePos);
}

void Sidebar::mouseDown() {
    sliderManager.mouseDown();
    buttonManager.mouseDown();
}

void Sidebar::mouseUp() {
    sliderManager.mouseUp();
    buttonManager.mouseUp();
}

#pragma endregion

void Sidebar::submitUI(){
    rpmSlider = new Slider(
        [&](){
            return Vector2(*scrW-sidebarWidth, margin+25) + Vector2(margin,margin);
        },
        [&](){
            return Vector2(sidebarWidth-2*margin, 30);
        },
        0,
        240,
        30
    );
    auto sldstl = Slider::Style::Windows10();
    rpmSlider->style = sldstl;
    sliderManager.registerSlider(rpmSlider);

    auto but_cam_persp = new Button(
        [&](){
            return Vector2(*scrW-sidebarWidth, 25+25+30+margin*2) + Vector2(margin, margin);
        },
        [&](){
            return Vector2(sidebarWidth-2*margin, 30);
        },
        UIPlacement::TOP_LEFT,
        "Perspectiva Livre",
        [&](Button*){
            manager->setCameraMode(Manager::CameraMode::PERSPECTIVE_FREE);
        }
    );
    but_cam_persp->style = ButtonStyle::Windows10();
    buttonManager.registerButton(but_cam_persp);

    auto but_cam_ortho_front = new Button(
        [&](){
            return Vector2(*scrW-sidebarWidth, 25+25+30+30+margin*3) + Vector2(margin, margin);
        },
        [&](){
            return Vector2(sidebarWidth-2*margin, 30);
        },
        UIPlacement::TOP_LEFT,
        "Ortografica Frente",
        [&](Button*){
            manager->setCameraMode(Manager::CameraMode::ORTHO_FRONT);
        }
    );
    but_cam_ortho_front->style = ButtonStyle::Windows10();
    buttonManager.registerButton(but_cam_ortho_front);

    auto but_cam_ortho_side = new Button(
        [&](){
            return Vector2(*scrW-sidebarWidth, 25+25+30+30+30+margin*4) + Vector2(margin, margin);
        },
        [&](){
            return Vector2(sidebarWidth-2*margin, 30);
        },
        UIPlacement::TOP_LEFT,
        "Ortografica Lado",
        [&](Button*){
            manager->setCameraMode(Manager::CameraMode::ORTHO_SIDE);
        }
    );
    but_cam_ortho_side->style = ButtonStyle::Windows10();
    buttonManager.registerButton(but_cam_ortho_side);

    auto but_cam_ortho_top = new Button(
        [&](){
            return Vector2(*scrW-sidebarWidth, 25+25+30+30+30+30+margin*5) + Vector2(margin, margin);
        },
        [&](){
            return Vector2(sidebarWidth-2*margin, 30);
        },
        UIPlacement::TOP_LEFT,
        "Ortografica Topo",
        [&](Button*){
            manager->setCameraMode(Manager::CameraMode::ORTHO_TOP);
        }
    );
    but_cam_ortho_top->style = ButtonStyle::Windows10();
    buttonManager.registerButton(but_cam_ortho_top);
}

float Sidebar::getRpm(){
    return rpmSlider->getValue();
}
