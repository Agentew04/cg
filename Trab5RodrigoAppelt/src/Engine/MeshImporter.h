#ifndef __OBJLOADER_H__
#define __OBJLOADER_H__

#include <string>
#include <map>
#include <memory>

#include "../Math/Vector3.h"

#include "Mesh.h"

namespace Engine {

    /// @brief Classe para ler arquivos .obj e .mtl e carregar materiais e texturas
    class MeshImporter {
    public:
        
        static std::shared_ptr<Engine::Mesh> loadMesh(std::string path);

        static std::map<std::string, Engine::Material> loadMaterials(std::string path);

    private:
    };
};
#endif // __OBJLOADER_H__