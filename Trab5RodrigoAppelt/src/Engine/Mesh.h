#ifndef __MESH_H__
#define __MESH_H__

#include <vector>
#include <string>

#include "../Math/Vector3.h"
#include "Material.h"

typedef unsigned int GLuint;

namespace Engine
{

   /// @brief Classe que representa uma mesh
   /// gerenciada pela engine.
   class Mesh
   {
   public:
      /// @brief Cria uma mesh a partir de um sweep rotacional. Suporta apenas meshes convexas.
      /// Normais sao suaves.
      /// @param profile A lista de pontos que formam o perfil da mesh.
      /// @param radialResolution Quantos vezes o perfil eh rotacionado para formar a mesh
      /// @return A mesh criada.
      static Mesh fromSweep(std::vector<Vector3> profile, unsigned int radialResolution);

      /// @brief Cria um tetrahedro normalizado regular
      /// @return A mesh do tetrahedro
      static Mesh tetrahedron(int resolution);

      /// @brief Carrega uma textura png no openGL
      /// @param path O caminho da textura
      /// @return O id da textura
      static GLuint loadTexture(std::string path);

      /// @brief Estrutura que representa uma face
      struct Face
      {
      public:
         /// @brief O indice de cada vertice que compoe a face
         std::vector<int> vertices;
         /// @brief O indice de cada normal de cada vertice
         std::vector<int> normals;
         std::vector<int> uv;
         int material;
      };

      /// @brief Lista de vertices do objeto
      std::vector<Vector3> vertexList;
      /// @brief Lista com a normais do objeto
      std::vector<Vector3> normalList;
      /// @brief Lista com as coordenadas de textura para cada vertice.
      std::vector<Vector2> uvList;
      /// @brief Lista de faces. Cada face eh composta por 3 vertices.
      std::vector<Face> faceList;

      std::vector<Material> materials;
   };
};

#endif