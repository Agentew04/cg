#include "MeshRenderer.h"

#include <GL/glut.h>

using namespace Engine::Components;

void MeshRenderer::Render() {
    glBegin(GL_TRIANGLES);
    for (auto face : mesh->faceList) {
        mesh->materials[face.material].use();
        for (int i = 0; i < face.vertices.size(); i++) {
            auto vertex = mesh->vertexList[face.vertices[i]];
            auto normal = mesh->normalList[face.normals[i]];
            glNormal3f(normal.x, normal.y, normal.z);
            glVertex3f(vertex.x, vertex.y, vertex.z);
        }
    }

    glEnd();
}