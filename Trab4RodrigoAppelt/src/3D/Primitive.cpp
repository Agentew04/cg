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

    // normais
    for (const auto& vertex : sphere.vertexList) {
        sphere.normalList.push_back(vertex.normalized());
    }

    // faces
    for(int i = 0; i < verticalResolution; i++){
        for(int j = 0; j < horizontalResolution; j++){
            int a = i * horizontalResolution + j;
            int b = i * horizontalResolution + (j + 1) % horizontalResolution;
            int c = (i + 1) * horizontalResolution + j;
            int d = (i + 1) * horizontalResolution + (j + 1) % horizontalResolution;
            sphere.faceList.push_back({{a, b, d}, {a, b, d}});
            sphere.faceList.push_back({{a, d, c}, {a, d, c}});
        }
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
        int nextI = (i + 1) % resolution;
        sphere.edgeList.push_back({capIdx, 2*i});
        sphere.edgeList.push_back({2*i,2*(nextI)});
        sphere.edgeList.push_back({2*i, 2*i + 1});
        sphere.edgeList.push_back({2*i + 1, 2*(nextI) + 1});
        sphere.edgeList.push_back({2*i + 1, capIdx+1});
    }

    // normais
    for(int i = 0; i < resolution; i++){
        sphere.normalList.push_back(Vector3(cos(i * step), sin(i * step), 0));
    }
    sphere.normalList.push_back(Vector3(0, 0, -1));
    sphere.normalList.push_back(Vector3(0, 0, 1));

    // faces
    for(int i = 0; i < resolution; i++){
        int nextI = (i + 1) % resolution;
        sphere.faceList.push_back({ // bottom
            { capIdx, 2*i, 2*(nextI) }, 
            { resolution, resolution, resolution}
        });
        sphere.faceList.push_back({ // side trig
            { 2*i, 2*(nextI), 2*i+1 }, 
            { i, nextI, i}
        });
        sphere.faceList.push_back({ // side trig
            { 2*(nextI), 2*(nextI)+1, 2*i+1 }, 
            { nextI, nextI, i}
        });
        sphere.faceList.push_back({
            { capIdx+1, 2*i+1, 2*(nextI)+1 },
            { resolution+1, resolution+1, resolution+1 }
        });

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

    // normais
    cube.normalList.push_back(Vector3(0, 0, -1));
    cube.normalList.push_back(Vector3(0, 0, 1));
    cube.normalList.push_back(Vector3(-1, 0, 0));
    cube.normalList.push_back(Vector3(1, 0, 0));
    cube.normalList.push_back(Vector3(0, -1, 0));
    cube.normalList.push_back(Vector3(0, 1, 0));

    // faces
    cube.faceList.push_back({ { 0, 1, 2, 3 }, { 0, 0, 0, 0 } });
    cube.faceList.push_back({ { 4, 5, 6, 7 }, { 1, 1, 1, 1 } });
    cube.faceList.push_back({ { 0, 4, 7, 3 }, { 2, 2, 2, 2 } });
    cube.faceList.push_back({ { 1, 5, 6, 2 }, { 3, 3, 3, 3 } });
    cube.faceList.push_back({ { 0, 1, 5, 4 }, { 4, 4, 4, 4 } });
    cube.faceList.push_back({ { 3, 2, 6, 7 }, { 5, 5, 5, 5 } });
    
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

    Vector3 v1 = (gear.vertexList[2] - gear.vertexList[0]).normalized();
    Vector3 v2 = (gear.vertexList[4] - gear.vertexList[2]).normalized();
    float teethDescentAngle = acos(v1.dot(v2));

    // normais
    for(int i=0; i<teeth; i++){
        for(int j=0; j<4; j++){
            float angle = i*bigStep + j*smallStep;
            
            if(j == 0 || j == 2){
                float x = cos(angle);
                float y = sin(angle);

                gear.normalList.push_back(Vector3(x, y, 0));
            }else{
                if(j == 1){
                    Vector3 lastNormal = gear.normalList.back();
                    auto rotatedNormal = Vector3(cos(teethDescentAngle) * lastNormal.x - sin(teethDescentAngle) * lastNormal.y, sin(teethDescentAngle) * lastNormal.x + cos(teethDescentAngle) * lastNormal.y, lastNormal.z);
                    gear.normalList.push_back(rotatedNormal);
                }else{
                    Vector3 lastNormal = gear.normalList.back();
                    auto rotatedNormal = Vector3(cos(-teethDescentAngle) * lastNormal.x - sin(-teethDescentAngle) * lastNormal.y, sin(-teethDescentAngle) * lastNormal.x + cos(-teethDescentAngle) * lastNormal.y, lastNormal.z);
                    gear.normalList.push_back(rotatedNormal);
                }
            }
        }
    }
    int topCapNormalIdx = teeth*4;
    int bottomCapNormalIdx = teeth*4 + 1;
    gear.normalList.push_back(Vector3(0, 0, 1)); // top cap
    gear.normalList.push_back(Vector3(0, 0, -1)); // bottom cap

    // faces
    for(int i=0; i<teeth; i++){
        for(int j=0; j<4; j++){
            int currentIdx = 8 * i + 2 * j;
            int currentNormal = 4 * i + j;
            int nextIdx = (currentIdx + 2) % (2 * teeth * 4);

            gear.faceList.push_back({
                {topCapIdx, currentIdx, nextIdx},
                {topCapNormalIdx, topCapNormalIdx, topCapNormalIdx}
            });
            gear.faceList.push_back({
                {bottomCapIdx, currentIdx+1, nextIdx+1},
                {bottomCapNormalIdx, bottomCapNormalIdx, bottomCapNormalIdx}
            });

            gear.faceList.push_back({
                {currentIdx, currentIdx+1, nextIdx},
                {currentNormal, currentNormal, currentNormal}
            });
            gear.faceList.push_back({
                {currentIdx+1, nextIdx, nextIdx+1},
                {currentNormal, currentNormal, currentNormal}
            });
        }
    }

    return gear;
}

void Primitive::Triangulate(){
    std::vector<Face> newFaceList;
    std::vector<std::vector<int>> newEdgeList;
    for(size_t i=0; i<faceList.size(); i++){
        Face face = faceList[i];

        if(face.vertices.size() == 3){
            newFaceList.push_back(face);

            continue;
        }

        for(int i = 1; i < face.vertices.size() - 1; i++){
            newFaceList.push_back({{face.vertices[0], face.vertices[i], face.vertices[i+1]}, {face.normals[0], face.normals[i], face.normals[i+1]}});
        }
    }
    faceList = newFaceList;
}