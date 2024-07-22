#ifndef __TERRAIN_BEZIER_H__
#define __TERRAIN_BEZIER_H__

#include "Component.h"

#include <vector>
#include "../../Math/Vector3.h"

namespace Engine::Components {
    /// @brief Componente que gera um terreno feito de nxn patches de bezier grau 3.
    class TerrainBezier : public Component {
    public:
        std::string getName() const override { return "TerrainBezier"; }

        /// @brief Gera todos os pontos de controle
        void Start() override;
        
        /// @brief Libera todos os recursos
        void Destroy() override;

        /// @brief Renderiza o terreno
        void Render() override;

        /// @brief Retorna a altura de um ponto no terreno.
        /// Nao usa coordenadas globais mas leva em conta a escala do terreno.
        /// @param x A coordenada X no terreno. Coordenadas locais do terreno considerando a escala.
        /// @param z A coordenada Z no terreno. Coordenadas locais do terreno considerando a escala.
        /// @return A altura global do terreno.
        float getHeightAt(float x, float z) const;

        int resolution;
    private:
        bool initialized = false;
        std::vector<Vector3> controlPoints;
        uint32_t textureId;
        std::vector<Vector3> terrainPoints;
        std::vector<Vector3> terrainNormals;

        void loadTexture();

        /// @brief Renderiza o terreno sem textura
        void render_internal(bool useTexture);
    };
};

#endif