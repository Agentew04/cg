#ifndef __MATERIAL_H__
#define __MATERIAL_H__

namespace Engine
{
   /// @brief Representa um material com workflow Specular.
   /// Facilmente representado num arquivo .mtl.
   class Material
   {
   public:
      float ambient[4] = {0.1f, 0.1f, 0.1f, 1.0f};
      float diffuse[4] = {0.8f, 0.8f, 0.8f, 1.0f};
      float specular[4] = {0.0f, 0.0f, 0.0f, 1.0f};
      float shininess = 0.0f;

      void setAmbient(float r, float g, float b, float a);
      void setDiffuse(float r, float g, float b, float a);
      void setSpecular(float r, float g, float b, float a);

      Material() = default;
      Material(float ambient[4], float diffuse[4], float specular[4], float shininess);

      /// @brief Aplica o material na maquina de estados do OpenGL
      void use();
   };
};

#endif