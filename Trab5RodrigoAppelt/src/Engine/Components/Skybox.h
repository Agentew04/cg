#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include "Component.h"

typedef unsigned int GLuint;

namespace Engine::Components{
    class Skybox : public Component {
    public:
        std::string name = "Skybox";
        void Start() override;
        void Update() override;
        void Destroy() override;
        void Render() override;


        std::string skyboxPath = ".\\Trab5RodrigoAppelt\\assets\\images\\";
        float distance = 500;
        GLuint leftTextureId;
        GLuint rightTextureId;
        GLuint topTextureId;
        GLuint bottomTextureId;
        GLuint frontTextureId;
        GLuint backTextureId;
    private:
        GLuint loadTexture(const std::string& path);
    };
};

#endif