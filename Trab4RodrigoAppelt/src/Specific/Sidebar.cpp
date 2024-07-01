#include "SideBar.h"

#include <iomanip>
#include <sstream>

#include "../gl_canvas2d.h"
#include "../Math/Vector2.h"
#include "Manager.h"

Sidebar::Sidebar(int* screenWidth, int* screenHeight, Manager* manager)
    : scrW(screenWidth), scrH(screenHeight), manager(manager) {
    submitUI();
}

#pragma region Events

void Sidebar::update(float) {

}

void Sidebar::render() {
    CV::translate(*scrW-sidebarWidth, 0);
    CV::color(sidebarColor);
    CV::rectFill(Vector2::zero(), Vector2(sidebarWidth, *scrH));

    // rpm controls
    CV::color(Vector3::fromHex(0x000000));
    std::stringstream ss;
    ss << std::fixed << std::setprecision(2) << rpmSlider->getValue();
    std::string rpmtext = "RPM: " + ss.str();
    CV::text(Vector2(margin,margin), rpmtext, 25, FontName::JetBrainsMono, UIPlacement::TOP_LEFT);
    sliderManager.draw();

    // camera controls
    CV::translate(*scrW-sidebarWidth, 2*20+margin*2);
    CV::color(Vector3::fromHex(0x000000));
    CV::text(Vector2(margin,margin), "Camera", 25, FontName::JetBrainsMono, UIPlacement::TOP_LEFT);
    buttonManager.draw();

    // visibilidade controls
    CV::translate(*scrW-sidebarWidth, 3*20 + 4*30 + 6*margin);
    CV::color(Vector3::fromHex(0x000000));
    CV::text(Vector2(margin,margin), "Visibilidade", 25, FontName::JetBrainsMono, UIPlacement::TOP_LEFT);
    checkboxManager.draw();

    // eixo cardan controls
    CV::translate(*scrW-sidebarWidth, 5*15 + 4*20 + 4*30 + 11*margin);
    CV::color(Vector3::fromHex(0x000000));
    CV::text(Vector2(margin,margin), "Eixo Cardan", 25, FontName::JetBrainsMono, UIPlacement::TOP_LEFT);
    std::stringstream ss2;
    ss2 << std::fixed << std::setprecision(2) << driveshaftAngleSlider->getValue() * (180/PI);
    std::string driveshafttext = "Angulo: " + ss2.str() + " graus";
    CV::translate(*scrW-sidebarWidth, 5*15 + 5*20 + 4*30 + 12*margin);
    CV::text(Vector2(margin,margin), driveshafttext, 25, FontName::JetBrainsMono, UIPlacement::TOP_LEFT);

    // rasterizador controls
    CV::translate(*scrW-sidebarWidth, 5*15 + 7*20 + 4*30 + 14*margin);
    CV::text(Vector2(margin,margin), "Rasterizador", 25, FontName::JetBrainsMono, UIPlacement::TOP_LEFT);

    CV::translate(*scrW-sidebarWidth, 6*15 + 8*20 + 7*30 + 19*margin);
    CV::text(Vector2(margin,margin), "Intensidade Luz Ambiente", 25, FontName::JetBrainsMono, UIPlacement::TOP_LEFT);

    CV::translate(*scrW-sidebarWidth, 6*15 + 10*20 + 7*30 + 21*margin);
    std::stringstream ss3;
    ss3 << std::fixed << std::setprecision(2) << upscaleSlider->getValue()+1;
    std::string upscaleText = "Upscale: " + ss3.str() + "x";
    CV::text(Vector2(margin,margin), upscaleText, 25, FontName::JetBrainsMono, UIPlacement::TOP_LEFT);
}

void Sidebar::updateMousePos(Vector2 mousePos) {
    this->mousePos = mousePos;
    sliderManager.updateMousePos(mousePos);
    buttonManager.updateMousePos(mousePos);
    checkboxManager.updateMousePos(mousePos);
}

void Sidebar::mouseDown() {
    sliderManager.mouseDown();
    buttonManager.mouseDown();
    checkboxManager.mouseDown();
}

void Sidebar::mouseUp() {
    sliderManager.mouseUp();
    buttonManager.mouseUp();
    checkboxManager.mouseUp();
}

#pragma endregion

void Sidebar::submitUI(){
    rpmSlider = new Slider(
        [&](){
            return Vector2(*scrW-sidebarWidth, 20+margin) + Vector2(margin,margin);
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
            return Vector2(*scrW-sidebarWidth, 3*20 + 2*margin) + Vector2(margin, margin);
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
            return Vector2(*scrW-sidebarWidth, 3*20 + 1*30 + 3*margin) + Vector2(margin, margin);
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
            return Vector2(*scrW-sidebarWidth, 3*20 + 2*30 + 4*margin) + Vector2(margin, margin);
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
            return Vector2(*scrW-sidebarWidth, 3*20 + 3*30 + 5*margin) + Vector2(margin, margin);
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

    auto chk_vis_crankshaft = new Checkbox(
        [&](){
            return Vector2(*scrW-sidebarWidth, 4*20 + 4*30 + 7*margin) + Vector2(margin, margin);
        },
        [&](){
            return Vector2(sidebarWidth-2*margin, 15);
        },
        "Virabrequim",
        true
    );
    chk_vis_crankshaft->setCallback([&](bool value){
        manager->setVisibility(Manager::SimulationPart::CRANKSHAFT, value);
    });
    auto chk_stl = Checkbox::Style::Windows10();
    chk_vis_crankshaft->style = chk_stl;
    checkboxManager.registerCheckbox(chk_vis_crankshaft);

    auto chk_vis_pistonarm = new Checkbox(
        [&](){
            return Vector2(*scrW-sidebarWidth, 1*15 + 4*20 + 4*30 + 8*margin) + Vector2(margin, margin);
        },
        [&](){
            return Vector2(sidebarWidth-2*margin, 15);
        },
        "Pistao",
        true
    );
    chk_vis_pistonarm->setCallback([&](bool value){
        manager->setVisibility(Manager::SimulationPart::PISTON_ARM, value);
    });
    chk_vis_pistonarm->style = chk_stl;
    checkboxManager.registerCheckbox(chk_vis_pistonarm);

    auto chk_vis_pistonbase = new Checkbox(
        [&](){
            return Vector2(*scrW-sidebarWidth, 2*15 + 4*20 + 4*30 + 9*margin) + Vector2(margin, margin);
        },
        [&](){
            return Vector2(sidebarWidth-2*margin, 15);
        },
        "Cilindro",
        true
    );
    chk_vis_pistonbase->setCallback([&](bool value){
        manager->setVisibility(Manager::SimulationPart::PISTON_BASE, value);
    });
    chk_vis_pistonbase->style = chk_stl;
    checkboxManager.registerCheckbox(chk_vis_pistonbase);

    auto chk_vis_gears = new Checkbox(
        [&](){
            return Vector2(*scrW-sidebarWidth, 3*15 + 4*20 + 4*30 + 10*margin) + Vector2(margin, margin);
        },
        [&](){
            return Vector2(sidebarWidth-2*margin, 15);
        },
        "Engrenagens",
        true
    );
    chk_vis_gears->setCallback([&](bool value){
        manager->setVisibility(Manager::SimulationPart::GEARS, value);
    });
    chk_vis_gears->style = chk_stl;
    checkboxManager.registerCheckbox(chk_vis_gears);

    auto chk_vis_driveshaft = new Checkbox(
        [&](){
            return Vector2(*scrW-sidebarWidth, 4*15 + 4*20 + 4*30 + 11*margin) + Vector2(margin, margin);
        },
        [&](){
            return Vector2(sidebarWidth-2*margin, 15);
        },
        "Eixo Cardan",
        true
    );
    chk_vis_driveshaft->setCallback([&](bool value){
        manager->setVisibility(Manager::SimulationPart::DRIVESHAFT, value);
    });
    chk_vis_driveshaft->style = chk_stl;
    checkboxManager.registerCheckbox(chk_vis_driveshaft);

    driveshaftAngleSlider = new Slider(
        [&](){
            return Vector2(*scrW-sidebarWidth, 5*15 + 6*20 + 4*30 + 13*margin) + Vector2(margin,margin);
        },
        [&](){
            return Vector2(sidebarWidth-2*margin, 30);
        },
        0.0f,
        PI * 0.25f, // 45 graus
        0.0f
    );
    driveshaftAngleSlider->style = sldstl;
    sliderManager.registerSlider(driveshaftAngleSlider);

    auto chk_raster_enable = new Checkbox(
        [&](){
            return Vector2(*scrW-sidebarWidth, 5*15 + 8*20 + 4*30 + 15*margin) + Vector2(margin, margin);
        },
        [&](){
            return Vector2(sidebarWidth-2*margin, 15);
        },
        "Rasterizador",
        false
    );
    chk_raster_enable->setCallback([&](bool value){
        manager->setRenderingMode(value ? Manager::RenderingMode::SOLID_PIXEL : Manager::RenderingMode::WIREFRAME);
    });
    chk_raster_enable->style = chk_stl;
    checkboxManager.registerCheckbox(chk_raster_enable);

    auto but_raster_color = new Button(
        [&](){
            return Vector2(*scrW-sidebarWidth, 6*15 + 8*20 + 4*30 + 16*margin) + Vector2(margin, margin);
        },
        [&](){
            return Vector2(sidebarWidth-2*margin, 30);
        },
        UIPlacement::TOP_LEFT,
        "Mostrar Color Buffer",
        [&](Button*){
            manager->setDisplayBuffer(Manager::DisplayBuffer::COLOR);
        }
    );
    but_raster_color->style = ButtonStyle::Windows10();
    buttonManager.registerButton(but_raster_color);

    auto but_raster_depth = new Button(
        [&](){
            return Vector2(*scrW-sidebarWidth, 6*15 + 8*20 + 5*30 + 17*margin) + Vector2(margin, margin);
        },
        [&](){
            return Vector2(sidebarWidth-2*margin, 30);
        },
        UIPlacement::TOP_LEFT,
        "Mostrar Depth Buffer",
        [&](Button*){
            manager->setDisplayBuffer(Manager::DisplayBuffer::DEPTH);
        }
    );
    but_raster_depth->style = ButtonStyle::Windows10();
    buttonManager.registerButton(but_raster_depth);

    auto but_raster_normal = new Button(
        [&](){
            return Vector2(*scrW-sidebarWidth, 6*15 + 8*20 + 6*30 + 18*margin) + Vector2(margin, margin);
        },
        [&](){
            return Vector2(sidebarWidth-2*margin, 30);
        },
        UIPlacement::TOP_LEFT,
        "Mostrar Normais Interpoladas",
        [&](Button*){
            manager->setDisplayBuffer(Manager::DisplayBuffer::NORMALS);
        }
    );
    but_raster_normal->style = ButtonStyle::Windows10();
    buttonManager.registerButton(but_raster_normal);

    ambientLightSlider = new Slider(
        [&](){
            return Vector2(*scrW-sidebarWidth, 6*15 + 9*20 + 7*30 + 20*margin) + Vector2(margin,margin);
        },
        [&](){
            return Vector2(sidebarWidth-2*margin, 30);
        },
        0.0f,
        1.0f,
        0.2f
    );
    ambientLightSlider->style = sldstl;
    sliderManager.registerSlider(ambientLightSlider);

    upscaleSlider = new Slider(
        [&](){
            return Vector2(*scrW-sidebarWidth, 6*15 + 11*20 + 7*30 + 22*margin) + Vector2(margin,margin);
        },
        [&](){
            return Vector2(sidebarWidth-2*margin, 30);
        },
        0.0f,
        7.0f,
        0.0f
    );
    upscaleSlider->style = sldstl;
    sliderManager.registerSlider(upscaleSlider);
}

float Sidebar::getRpm(){
    if(rpmSlider != nullptr){
        return rpmSlider->getValue();
    }else{
        std::cout << "RPM Slider eh null!" << std::endl;
        return 0.0f;
    }
}

float Sidebar::getDriveshaftAngle(){
    if(driveshaftAngleSlider != nullptr){
        return driveshaftAngleSlider->getValue();
    }else{
        std::cout << "Driveshaft Angle Slider eh null!" << std::endl;
        return 0.0f;
    }
}

float Sidebar::getAmbientLight(){
    if(ambientLightSlider != nullptr){
        return ambientLightSlider->getValue();
    }else{
        std::cout << "Ambient Light Slider eh null!" << std::endl;
        return 0.0f;
    }
}

float Sidebar::getUpscaleFactor(){
    if(upscaleSlider != nullptr){
        return upscaleSlider->getValue()+1;
    }else{
        std::cout << "Upscale Slider eh null!" << std::endl;
        return 1.0f;
    }
}

float Sidebar::getSidebarWidth(){
    return sidebarWidth;
}
