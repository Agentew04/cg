#ifndef __TERRAIN_H__
#define __TERRAIN_H__

#include <string>
#include <memory>

#include "Component.h"

namespace Engine::Components {

    typedef unsigned int GLuint;

    class Terrain : public Component {
    public:
        std::string getName() const override { return "Terrain"; }
        void Start() override;
        void Update(float delta) override;
        void Destroy() override;
        void Render() override;
    
        /// @brief O numero de pontos do lado do terreno.
        /// Deve atender o requisito: (N-4)%3==0
        int resolution;
        std::unique_ptr<short> heightmap;
        GLuint textureId;
    };
} // namespace Engine::Components

#endif