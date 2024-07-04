#ifndef __MESH_RENDERER_H__
#define __MESH_RENDERER_H__

#include <memory>

#include "Component.h"
#include "../Mesh.h"
#include "../../Math/Vector3.h"

namespace Engine::Components {

    /// @brief Componente responsavel por renderizar uma mesh.
    class MeshRenderer : public Component {
    public:
        std::shared_ptr<Mesh> mesh;
        Vector3 color;

        std::string getName() const override { return "MeshRenderer"; }

        void Start() override;
        void Update(float delta) override;
        void Destroy() override;
        void Render() override;
    
    private:
    };
};

#endif