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
        void Start() override;
        void Update(float delta) override;
        void Destroy() override;
        void Render() override;

        Vector3 position;
        Vector3 rotation;
        ViewMode viewMode;
        bool isActive;
    };
};

#endif