#ifndef __OBJLOADER_H__
#define __OBJLOADER_H__

#include <string>
#include <map>
#include <memory>

#include "../Math/Vector3.h"

#include "Mesh.h"

namespace Engine
{

   /// @brief Classe para ler arquivos .obj e .mtl e carregar materiais e texturas
   class MeshImporter
   {
   public:
      /// @brief Carrega um mesh de um arquivo .obj
      static std::shared_ptr<Mesh> loadMesh(std::string path);

      /// @brief Carrega varios materiais nomeados de um
      /// arquivo .mtl
      static std::map<std::string, Material> loadMaterials(std::string path);

   private:
   };
};
#endif // __OBJLOADER_H__