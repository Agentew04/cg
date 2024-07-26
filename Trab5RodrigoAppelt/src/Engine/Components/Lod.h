#ifndef __LOD_H__
#define __LOD_H__

#include "Component.h"
#include "MeshRenderer.h"

namespace Engine::Components {

    /// @brief Classe que gerencia um MeshRenderer e muda entre
    /// varios modelos com niveis de detalhes diferentes
    class Lod : public Component {
        public:

            std::string getName() const override { return "Lod"; }
            void Start() override;
            void Update(float delta) override;

            /// @brief Lista de meshes com diferentes niveis de detalhe.
            /// Deve ter um item a mais do que o vetor distances
            std::vector<std::shared_ptr<Mesh>> meshes;
            /// @brief Lista de distancias para cada nivel de detalhe.
            /// Deve ter um item a menos do que o vetor meshes
            std::vector<float> distances;
        private:
            /// @brief Numero de distancias. Um a menos do numero de meshes
            int levels;
            std::shared_ptr<MeshRenderer> meshRenderer;

    };
};

#endif