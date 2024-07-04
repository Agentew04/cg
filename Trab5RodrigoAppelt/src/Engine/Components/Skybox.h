#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include "Component.h"
#include "../../Math/Vector3.h"

typedef unsigned int GLuint;

namespace Engine::Components{
    class Skybox : public Component {
    public:
        std::string getName() const override { return "Skybox"; }
        void Start() override;
        void Update(float delta) override;
        void Destroy() override;
        void Render() override;


        float distance = 500;
    private:
        GLuint loadTexture(const std::string& path);
        std::string skyboxPath = ".\\Trab5RodrigoAppelt\\assets\\images\\";
        GLuint leftTextureId;
        GLuint rightTextureId;
        GLuint topTextureId;
        GLuint bottomTextureId;
        GLuint frontTextureId;
        GLuint backTextureId;

        Vector3 camPos;
    };
};

#endif
