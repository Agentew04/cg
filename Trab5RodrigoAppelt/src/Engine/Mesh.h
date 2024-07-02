#ifndef __MESH_H__
#define __MESH_H__

#include <vector>
#include "../Math/Vector3.h"

namespace Engine {

    /// @brief Classe que representa uma mesh
    /// gerenciada pela engine.
    class Mesh {
    public:
        /// @brief Cria uma mesh a partir de um sweep rotacional.
        /// @param profile A lista de pontos que formam o perfil da mesh.
        /// @param radialResolution Quantos vezes o perfil eh rotacionado para formar a mesh
        /// @return A mesh criada.
        static Mesh fromSweep(std::vector<Vector3> profile, int radialResolution);
    };
};

#endif