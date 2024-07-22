#include "MeshRenderer.h"

#include <GL/glut.h>


using namespace Engine::Components;

void MeshRenderer::Render() {
    if(!mesh){
        //log(LogLevel::WARNING, "MeshRenderer has no mesh");
        return;
    }

    Material defaultMat;
    defaultMat.setDiffuse(1,0,0,1);
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
            glNormal3f(normal.x, normal.y, normal.z);
            glVertex3f(vertex.x, vertex.y, vertex.z);
        }
    }

    glEnd();
}
