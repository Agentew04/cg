#include "MeshRenderer.h"

#include <GL/glut.h>


#ifdef ERROR
#undef ERROR
#endif

using namespace Engine::Components;

void MeshRenderer::Render() {
    if(!mesh){
        //log(LogLevel::WARNING, "MeshRenderer has no mesh");
        return;
    }

    Material defaultMat;
    defaultMat.setDiffuse(1,0,0,1);

    if(textureId != 0){
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureId);
    }

    glBegin(GL_TRIANGLES);
    for (auto face : mesh->faceList) {
        if(face.material >= 0 && mesh->materials.size()>=(size_t)face.material+1){
        mesh->materials[face.material].use();
        }else{
            defaultMat.use();
        }
        for (size_t i = 0; i < face.vertices.size(); i++) {
            auto vertex = mesh->vertexList[face.vertices[i]];
            auto normal = mesh->normalList[face.normals[i]];
            // if(textureId != 0){
                auto uv = mesh->uvList[face.uv[i]];
                glTexCoord2f(uv.x, uv.y);
            // }
            glNormal3f(normal.x, normal.y, normal.z);
            glVertex3f(vertex.x, vertex.y, vertex.z);
        }
    }
    glEnd();

    if(textureId != 0){
        glDisable(GL_TEXTURE_2D);
    }
}

void MeshRenderer::useTexture(GLuint textureId){
    this->textureId = textureId;
    log(LogLevel::INFO, "Using texture: " + std::to_string(textureId));
}

void MeshRenderer::Destroy(){
    if(textureId != 0) {
        log(LogLevel::INFO, "Deleting texture: " + std::to_string(textureId));
        glDeleteTextures(1, &textureId);
    }
}