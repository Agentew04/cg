#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#include <string>
#include <memory>

#include "Component.h"
#include "../../Math/Vector3.h"

namespace Engine::Components {

    typedef unsigned int GLuint;

    class Terrain : public Component {
    public:
        std::string getName() const override { return "Terrain"; }

        /// @brief Cria um heightmap aleatorio
        void Start() override;
        void Destroy() override;
        void Render() override;


        float* controlPoints;
        uint32_t textureId;
    private:
        void render_with_texture();
        void render_without_texture();
    };
} // namespace Engine::Components

#endif
