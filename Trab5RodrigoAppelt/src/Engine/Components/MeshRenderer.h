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
    /// Suporta texturas. O material usado eh guardado no objeto Engine::Mesh.
    class MeshRenderer : public Component {
    public:
        std::shared_ptr<Mesh> mesh;

        std::string getName() const override { return "MeshRenderer"; }

        void Render() override;
        void Destroy() override;

        /// @brief Usa uma textura para renderizar a mesh.
        /// @param textureId O id da textura. Ja deve ter sido carregada no OpenGL.
        void useTexture(GLuint textureId);
    private:
        GLuint textureId = 0;
    };
};

#endif
