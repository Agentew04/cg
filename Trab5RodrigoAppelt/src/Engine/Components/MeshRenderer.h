#ifndef __MESH_RENDERER_H__
#define __MESH_RENDERER_H__

#include <memory>
#include <string>

#include "Component.h"
#include "../Mesh.h"
#include "../../Math/Vector3.h"

typedef unsigned int GLuint;

namespace Engine::Components {

    /// @brief Componente responsavel por renderizar uma mesh.
    class MeshRenderer : public Component {
    public:
        std::shared_ptr<Mesh> mesh;

        std::string getName() const override { return "MeshRenderer"; }

        void Render() override;
        void Destroy() override;

        void useTexture(GLuint textureId);
    private:
        GLuint textureId = 0;
    };
};

#endif
