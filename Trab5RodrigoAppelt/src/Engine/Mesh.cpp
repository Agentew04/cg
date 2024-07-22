#include "Mesh.h"

#include <vector>
#include <cmath>

#include "../Math/Vector3.h"

#define M_PI 3.1415926535897932384626433832795
#define ONE_OVER_SQRT_2 1.414213562373095

using namespace Engine;

inline Vector2 sphereToUv(Vector3 vertex){
    float tetha = std::atan2(vertex.z, vertex.x);
    float phi = std::acos(vertex.y);
    float u = (tetha+M_PI)/(2*M_PI);
    float v = phi/M_PI;
    return Vector2(u,v);
}

std::vector<Mesh::Face> recFace(Mesh& m, Vector3 a, Vector3 b, Vector3 c, int depth, int resolution){
    if(depth >= resolution){
        // criar a face
        Mesh::Face f;
        a.normalize();
        b.normalize();
        c.normalize();
        m.vertexList.push_back(a);
        m.vertexList.push_back(b);
        m.vertexList.push_back(c);
        m.normalList.push_back(a);
        m.normalList.push_back(b);
        m.normalList.push_back(c);
        m.uvList.push_back(sphereToUv(a));
        m.uvList.push_back(sphereToUv(b));
        m.uvList.push_back(sphereToUv(c));

        f.vertices.push_back(m.vertexList.size()-3);
        f.vertices.push_back(m.vertexList.size()-2);
        f.vertices.push_back(m.vertexList.size()-1);
        f.normals = f.vertices;
        f.uv = f.vertices;
        f.material = 0;
        // normais sao iguais aos vertices
        // vertice - normal - uv sao relacoes 1 pra 1.
        return {f};
    }else{
        // recursivo para 4 novas faces
        Vector3 ab = (a+b)*0.5;
        Vector3 ac = (a+c)*0.5;
        Vector3 bc = (b+c)*0.5;

        auto f1 = recFace(m, a, ab, ac, depth+1, resolution);
        auto f2 = recFace(m, ab, b, bc, depth+1, resolution);
        auto f3 = recFace(m, ac, bc, c, depth+1, resolution);
        auto f4 = recFace(m, ab, bc, ac, depth+1, resolution);

        f1.insert(f1.end(), f2.begin(), f2.end());
        f1.insert(f1.end(), f3.begin(), f3.end());
        f1.insert(f1.end(), f4.begin(), f4.end());
        return f1;
    }
}

Mesh Mesh::tetrahedron(int resolution){
    Mesh m;

    Vector3 a(1,0,-ONE_OVER_SQRT_2);
    Vector3 b(-1,0,-ONE_OVER_SQRT_2);
    Vector3 c(0,1,ONE_OVER_SQRT_2);
    Vector3 d(0,-1,ONE_OVER_SQRT_2);

    // evaluate the 4 faces
    auto face1 = recFace(m, a, b, c, 1, resolution);
    auto face2 = recFace(m, a, b, d, 1, resolution);
    auto face3 = recFace(m, a, c, d, 1, resolution);
    auto face4 = recFace(m, b, c, d, 1, resolution);

    m.faceList.insert(m.faceList.end(), face1.begin(), face1.end());
    m.faceList.insert(m.faceList.end(), face2.begin(), face2.end());
    m.faceList.insert(m.faceList.end(), face3.begin(), face3.end());
    m.faceList.insert(m.faceList.end(), face4.begin(), face4.end());

    return m;
}
