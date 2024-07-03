#include "Material.h"

#include <GL/glut.h>

namespace Engine {

    Material::Material(float ambient[4], float diffuse[4], float specular[4], float shininess) {
        for (int i = 0; i < 4; i++) {
            this->ambient[i] = ambient[i];
            this->diffuse[i] = diffuse[i];
            this->specular[i] = specular[i];
        }
        this->shininess = shininess;
    }

    void Material::use() {
        glMaterialfv(GL_FRONT, GL_AMBIENT, ambient);
        glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
        glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
        glMaterialf(GL_FRONT, GL_SHININESS, shininess);
    }

    void Material::setAmbient(float r, float g, float b, float a) {
        ambient[0] = r;
        ambient[1] = g;
        ambient[2] = b;
        ambient[3] = a;
    }

    void Material::setDiffuse(float r, float g, float b, float a) {
        diffuse[0] = r;
        diffuse[1] = g;
        diffuse[2] = b;
        diffuse[3] = a;
    }

    void Material::setSpecular(float r, float g, float b, float a) {
        specular[0] = r;
        specular[1] = g;
        specular[2] = b;
        specular[3] = a;
    }
}