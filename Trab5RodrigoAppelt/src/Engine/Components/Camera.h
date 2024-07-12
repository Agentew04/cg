#ifndef __CAMERA_COMPONENT_H__
#define __CAMERA_COMPONENT_H__

#include "Component.h"
#include "../../Math/Vector3.h"

namespace Engine::Components {
    class Camera : public Component {
    public:

        enum class ViewMode {
            WIREFRAME,
            SOLID_SHADED
        };

        std::string getName() const override { return "Camera"; }
    
        bool isActive;
    };
};

#endif