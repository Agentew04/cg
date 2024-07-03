#ifndef __MATERIAL_H__
#define __MATERIAL_H__

namespace Engine {
    class Material {
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
        void use();
    };
};

#endif