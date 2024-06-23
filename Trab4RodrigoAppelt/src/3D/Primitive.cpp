#include "Primitive.h"

Primitive Primitive::createSphere(int horizontalResolution, int verticalResolution){
    Primitive sphere;

    float horizontalStep = 2 * 3.1415 / horizontalResolution;
    float verticalStep = 3.1415 / verticalResolution;

    // vertices
    for(int i = 0; i < verticalResolution+1; i++){
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

    // normais
    for (const auto& face : sphere.faceList) {
        Vector3 v0 = sphere.vertexList[face[0]];
        Vector3 v1 = sphere.vertexList[face[1]];
        Vector3 v2 = sphere.vertexList[face[2]];

        Vector3 edge1 = v1 - v0;
        Vector3 edge2 = v2 - v0;

        Vector3 normal = edge1.cross(edge2).normalized();
        sphere.normalList.push_back(normal);
    }

    return sphere;
}

Primitive Primitive::createCylinder(int resolution, float height, float radius){
    Primitive sphere;

    float step = 2 * 3.1415 / resolution;

    // vertices
    for(int i = 0; i < resolution; i++){
        float x = cos(i * step) * radius;
        float y = sin(i * step) * radius;
        sphere.vertexList.push_back(Vector3(x, y, -height/2));
        sphere.vertexList.push_back(Vector3(x, y, height/2));
    }
    sphere.vertexList.push_back(Vector3(0, 0, -height/2));
    sphere.vertexList.push_back(Vector3(0, 0, height/2));

    int capIdx = resolution * 2;

    // arestas
    for(int i = 0; i < resolution; i++){
        // centro de cima ate borda de cima
        sphere.edgeList.push_back({capIdx, i*2});
        // borda cima -> borda baixo
        sphere.edgeList.push_back({i*2, (i*2)+1});
        // borda baixo -> centro
        sphere.edgeList.push_back({(i*2)+1, capIdx+1});
        // borda cima -> borda cima lado
        sphere.edgeList.push_back({i*2, (i*2+2) % (resolution*2)});
        // borda baixo -> borda baixo lado
        sphere.edgeList.push_back({(i*2)+1, (i*2+3) % (resolution*2)});
    }

    // faces
    for(int i = 0; i < resolution; i++){
        // parte de cima
        sphere.faceList.push_back({capIdx, i*2, ((i+1)*2) % (resolution*2)});
        sphere.normalList.push_back(Vector3(0, 0, -1));
        // parte de baixo
        sphere.faceList.push_back({capIdx+1, (i*2)+1, ((i+1)*2+1) % (resolution*2)});
        sphere.normalList.push_back(Vector3(0, 0, 1));
        // lado(vao ser 2 triangulos)
        sphere.faceList.push_back({i*2, (i*2)+1, ((i+1)*2) % (resolution*2)});
        sphere.faceList.push_back({(i*2)+1, ((i+1)*2) % (resolution*2), ((i+1)*2+1) % (resolution*2)});
        sphere.normalList.push_back((sphere.vertexList[(i*2)+1] - sphere.vertexList[i*2]).cross(sphere.vertexList[((i+1)*2)] - sphere.vertexList[i*2]).normalized());
        sphere.normalList.push_back((sphere.vertexList[((i+1)*2)+1] - sphere.vertexList[(i*2)+1]).cross(sphere.vertexList[((i+1)*2)+1] - sphere.vertexList[((i+1)*2)]).normalized());
    }
    
    // normais
    
    return sphere;
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

    // normais
    cube.normalList.push_back(Vector3(0, 0, -1));
    cube.normalList.push_back(Vector3(0, 0, 1));
    cube.normalList.push_back(Vector3(0, -1, 0));
    cube.normalList.push_back(Vector3(0, 1, 0));
    cube.normalList.push_back(Vector3(-1, 0, 0));
    cube.normalList.push_back(Vector3(1, 0, 0));
    
    return cube;
}

Primitive Primitive::createGear(int teeth, float teethHeight){
    Primitive gear;

    float height = 0.5;

    float bigStep = (2*3.14159) / teeth;
    float smallStep = bigStep / 4;

    float outerRadius = 1 + teethHeight;
    float innerRadius = 1 - teethHeight;

    // vertices
    for(int i=0; i<teeth; i++){
        for(int j=0; j<4; j++){
            float angle = i*bigStep + j*smallStep;
            float radius = (j == 0 || j == 1) ? outerRadius : innerRadius;

            float x = radius * cos(angle);
            float y = radius * sin(angle);
            gear.vertexList.push_back(Vector3(x, y, height/2));
            gear.vertexList.push_back(Vector3(x, y, -height/2));
        }
    }
    gear.vertexList.push_back(Vector3(0,0,height/2)); // top cap
    gear.vertexList.push_back(Vector3(0,0,-height/2)); // bottom cap

    int topCapIdx = 2*teeth*4;
    int bottomCapIdx = 2*teeth*4 + 1;

    // arestas
    for(int i=0; i<teeth; i++){
        for(int j=0; j<4; j++){
            int currentIdx = 8 * i + 2 * j;
            int nextIdx = (currentIdx + 2) % (2 * teeth * 4);
            
            // Vertical edges
            gear.edgeList.push_back({currentIdx, currentIdx + 1});
            
            // Horizontal edges (top)
            gear.edgeList.push_back({currentIdx, nextIdx});
            
            // Horizontal edges (bottom)
            gear.edgeList.push_back({currentIdx + 1, nextIdx + 1});
            
            // Edges to center
            if (j == 3) { // connect last vertex of each tooth to center
                gear.edgeList.push_back({currentIdx, topCapIdx});
                gear.edgeList.push_back({currentIdx + 1, bottomCapIdx});
            }
        }
    }

    // faces
    for(int i=0; i<teeth; i++){
        for(int j=0; j<4; j++){
            int currentIdx = 8 * i + 2 * j;
            int nextIdx = (currentIdx + 2) % (2 * teeth * 4);

            // Create faces for the sides of the teeth
            gear.faceList.push_back({currentIdx, nextIdx, nextIdx + 1, currentIdx + 1});

            // Create faces for the top and bottom surfaces of the gear
            if (j == 3) {
                // Top face
                gear.faceList.push_back({topCapIdx, currentIdx, nextIdx});
                // Bottom face
                gear.faceList.push_back({bottomCapIdx, nextIdx + 1, currentIdx + 1});
            }
        }
    }

    return gear;
}

void Primitive::Triangulate(){
    std::vector<std::vector<int>> newFaceList;
    for(auto& face : faceList){
        for(int i = 1; i < face.size()-1; i++){
            newFaceList.push_back({face[0], face[i], face[i+1]});
        }
    }
    faceList = newFaceList;
}