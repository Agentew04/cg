#ifndef __CAMERA_COMPONENT_H__
#define __CAMERA_COMPONENT_H__

#include "Component.h"
#include "../../Math/Vector3.h"

namespace Engine::Components {

    /// @brief Componente que sinaliza que um ator eh uma camera.
    /// A logica de gluLookAt eh gerenciada na Engine.cpp.
    class Camera : public Component {
    public:

        std::string getName() const override { return "Camera"; }

        /// @brief Define se essa camera deve ser usada. Util
        /// para uma cena com varias camera. 
        /// @remarks Intuito principal era
        /// fazer multiplas perspectivas (1a, 3a) de um aviao.
        /// Ideia foi descartada por falta de tempo.
        bool isActive;
    };
};

#endif