#include "Primitive.h"

Primitive Primitive::createSphere(int horizontalResolution, int verticalResolution){
    Primitive sphere;

    float horizontalStep = 2 * 3.1415 / horizontalResolution;
    float verticalStep = 3.1415 / verticalResolution;

    // vertices
    for(int i = 0; i < verticalResolution; i++){
        for(int j = 0; j < horizontalResolution; j++){
            float x = cos(j * horizontalStep) * sin(i * verticalStep);
            float y = sin(j * horizontalStep) * sin(i * verticalStep);
            float z = cos(i * verticalStep);
            sphere.vertexList.push_back(Vector3(x, y, z));
        }
    }

    // arestas
    for(int i = 0; i < verticalResolution; i++){
        for(int j = 0; j < horizontalResolution; j++){
            int a = i * horizontalResolution + j;
            int b = i * horizontalResolution + (j + 1) % horizontalResolution;
            int c = (i + 1) * horizontalResolution + j;
            int d = (i + 1) * horizontalResolution + (j + 1) % horizontalResolution;
            sphere.edgeList.push_back({a, b});
            sphere.edgeList.push_back({a, c});
            sphere.edgeList.push_back({b, d});
            sphere.edgeList.push_back({c, d});
        }
    }

    // faces
    for(int i = 0; i < verticalResolution; i++){
        for(int j = 0; j < horizontalResolution; j++){
            int a = i * horizontalResolution + j;
            int b = i * horizontalResolution + (j + 1) % horizontalResolution;
            int c = (i + 1) * horizontalResolution + j;
            int d = (i + 1) * horizontalResolution + (j + 1) % horizontalResolution;
            sphere.faceList.push_back({a, b, d, c});
        }
    }
    return sphere;
}

Primitive Primitive::createCylinder(int resolution, float height, float radius){
    Primitive cylinder;

    float step = 2 * 3.1415 / resolution;

    // vertices
    for(int i = 0; i < resolution; i++){
        float x = cos(i * step) * radius;
        float y = sin(i * step) * radius;
        cylinder.vertexList.push_back(Vector3(x, y, height/2));
        cylinder.vertexList.push_back(Vector3(x, y, -height/2));
    }

    // arestas
    for(int i = 0; i < resolution; i++){
        int a = i;
        int b = (i + 1) % resolution;
        int c = i + resolution;
        int d = (i + 1) % resolution + resolution;
        cylinder.edgeList.push_back({a, b});
        cylinder.edgeList.push_back({c, d});
        cylinder.edgeList.push_back({a, c});
    }

    // faces
    for(int i = 0; i < resolution; i++){
        int a = i;
        int b = (i + 1) % resolution;
        int c = i + resolution;
        int d = (i + 1) % resolution + resolution;
        cylinder.faceList.push_back({a, b, d, c});
    }

    return cylinder;
}

Primitive Primitive::createCube(float size){
    Primitive cube;

    // vertices
    cube.vertexList.push_back(Vector3(-size/2, -size/2, -size/2));
    cube.vertexList.push_back(Vector3(size/2, -size/2, -size/2));
    cube.vertexList.push_back(Vector3(size/2, size/2, -size/2));
    cube.vertexList.push_back(Vector3(-size/2, size/2, -size/2));
    cube.vertexList.push_back(Vector3(-size/2, -size/2, size/2));
    cube.vertexList.push_back(Vector3(size/2, -size/2, size/2));
    cube.vertexList.push_back(Vector3(size/2, size/2, size/2));
    cube.vertexList.push_back(Vector3(-size/2, size/2, size/2));

    // arestas
    cube.edgeList.push_back({0, 1});
    cube.edgeList.push_back({1, 2});
    cube.edgeList.push_back({2, 3});
    cube.edgeList.push_back({3, 0});
    cube.edgeList.push_back({4, 5});
    cube.edgeList.push_back({5, 6});
    cube.edgeList.push_back({6, 7});
    cube.edgeList.push_back({7, 4});
    cube.edgeList.push_back({0, 4});
    cube.edgeList.push_back({1, 5});
    cube.edgeList.push_back({2, 6});
    cube.edgeList.push_back({3, 7});

    // faces
    cube.faceList.push_back({0, 1, 2, 3});
    cube.faceList.push_back({4, 5, 6, 7});
    cube.faceList.push_back({0, 4, 7, 3});
    cube.faceList.push_back({1, 5, 6, 2});
    cube.faceList.push_back({0, 1, 5, 4});
    cube.faceList.push_back({3, 2, 6, 7});

    return cube;
}

