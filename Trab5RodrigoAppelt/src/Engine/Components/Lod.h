#ifndef __LOD_H__
#define __LOD_H__

#include "Component.h"
#include "MeshRenderer.h"

namespace Engine::Components {
    class Lod : public Component {
        public:

            std::string getName() const override { return "Lod"; }
            void Start() override;
            void Update(float delta) override;

            std::vector<std::shared_ptr<Mesh>> meshes;
            std::vector<float> distances;
        private:
            /// @brief Numero de distancias. Um a menos do numero de meshes
            int levels;
            std::shared_ptr<MeshRenderer> meshRenderer;

    };
};

#endif