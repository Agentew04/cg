#include "Rasterizer.h"

#include <limits>

#include "Perspective.h"

void Rasterizer::Rasterize(
        std::vector<Primitive>& primitives, 
        Camera3D& camera,
        Buffer* colorBuffer,
        Buffer* depthBuffer,
        const Vector3& lightDirection,
        float scale){
    // define zbuffer como infinito
    depthBuffer->fill(std::numeric_limits<float>::infinity());

    float* zbuffer = depthBuffer->getBuffer();
    float* cbuf = colorBuffer->getBuffer();
    float colorBufferWidth, colorBufferHeight;
    colorBuffer->getSize(colorBufferWidth, colorBufferHeight);

    // transforma para posicao de camera e projecao
    for(auto &poly : primitives){
        poly.vertexList = camera.worldToCamera(poly.vertexList);
        poly.vertexList = P3D::perspectiveProjectionVector3(poly.vertexList, camera.getD());
        poly.vertexList = P3D::translateVector(poly.vertexList, Vector3(colorBufferWidth*0.5, colorBufferHeight*0.5, 0));

        for(auto it=poly.faceList.begin(); it!=poly.faceList.end(); it++){
            bool cull = false;   
            auto face = *it;
            for(auto &v : face){
                if(camera.isOnFrustumCS(poly.vertexList[v]) == false){
                    cull = true;
                    break;
                }
            }
            if(cull){
                it = poly.faceList.erase(it);
                it--;
            }
        }
    }
    
    Vector3 localLightDirection = camera.worldToCamera(lightDirection);


    // para cada face(poligono) de cada primitiva
    for(auto &poly: primitives){
        Vector3 color = poly.color;
        for(auto &face: poly.faceList){
            // calcula a bounding box
            Vector2 p1,p2;
            BoundBox(poly.vertexList, face, p1, p2);

            for(int y = p1.y; y <= p2.y; y++){
                if(y < 0 || y >= colorBufferHeight) continue;
                for(int x = p1.x; x <= p2.x; x++){
                    if(x < 0 || x >= colorBufferWidth) continue;
                    
                    // para cada pixel dentro do poligono
                    if(isInside(poly.vertexList, face, Vector2(x, y))){
                        // calcula o Z do pixel
                        float z = interpolateZ(poly.vertexList, face, Vector2(x, y));
                        // se o z é menor que o zbuffer
                        if(z < zbuffer[depthBuffer->getOffset(x, y)]){
                            // atualiza o zbuffer
                            zbuffer[depthBuffer->getOffset(x, y)] = z;
                            
                            // calcula a cor do pixel
                            //Vector3 normal = interpolateNormal(poly.vertexList, poly.normalList, face, Vector2(x, y));
                            //float intensity = std::max(0.0f, normal.dot(localLightDirection));
                            //Vector3 pixelcolor = color*intensity;
                            cbuf[colorBuffer->getOffset(x, y)] = color.x;
                            cbuf[colorBuffer->getOffset(x, y) + 1] = color.y;
                            cbuf[colorBuffer->getOffset(x, y) + 2] = color.z;
                        }
                    }
                }
            }
        }
    }

}

void Rasterizer::BoundBox(
    const std::vector<Vector3>& vertexList,
    const std::vector<int>& face,
    Vector2& p1,
    Vector2& p2){
    float minX = std::numeric_limits<float>::infinity();
    float minY = std::numeric_limits<float>::infinity();
    float maxX = -std::numeric_limits<float>::infinity();
    float maxY = -std::numeric_limits<float>::infinity();

    for(auto &vi : face){
        if(vertexList[vi].x < minX) minX = vertexList[vi].x;
        if(vertexList[vi].y < minY) minY = vertexList[vi].y;
        if(vertexList[vi].x > maxX) maxX = vertexList[vi].x;
        if(vertexList[vi].y > maxY) maxY = vertexList[vi].y;
    }
    p1 = Vector2(minX, minY);
    p2 = Vector2(maxX, maxY);
}

bool Rasterizer::isInside(
    const std::vector<Vector3>& vertexList,
    const std::vector<int>& face,
    Vector2 point){
    int n = face.size();
    bool inside = false;
    for(int i = 0, j = n - 1; i < n; j = i++){
        if(((vertexList[face[i]].y > point.y) != (vertexList[face[j]].y > point.y)) &&
            (point.x < (vertexList[face[j]].x - vertexList[face[i]].x) * (point.y - vertexList[face[i]].y) / (vertexList[face[j]].y - vertexList[face[i]].y) + vertexList[face[i]].x)){
            inside = !inside;
        }
    }
    return inside;
}

float Rasterizer::interpolateZ(
    const std::vector<Vector3>& vertexList,
    const std::vector<int>& face,
    Vector2 point){
    int n = face.size();
    float z = 0;
    for(int i = 0, j = n - 1; i < n; j = i++){
        if(((vertexList[face[i]].y > point.y) != (vertexList[face[j]].y > point.y)) &&
            (point.x < (vertexList[face[j]].x - vertexList[face[i]].x) * (point.y - vertexList[face[i]].y) / (vertexList[face[j]].y - vertexList[face[i]].y) + vertexList[face[i]].x)){
            float z1 = vertexList[face[i]].z;
            float z2 = vertexList[face[j]].z;
            z = z1 + (z2 - z1) * (point.x - vertexList[face[i]].x) / (vertexList[face[j]].x - vertexList[face[i]].x);
        }
    }
    return z;
}

float Rasterizer::interpolateNormal(
    const std::vector<Vector3>& vertexList,
    const std::vector<Vector3>& normalList,
    const std::vector<int>& face,
    Vector2 point){
    int n = face.size();
    float z = 0;
    for(int i = 0, j = n - 1; i < n; j = i++){
        if(((vertexList[face[i]].y > point.y) != (vertexList[face[j]].y > point.y)) &&
            (point.x < (vertexList[face[j]].x - vertexList[face[i]].x) * (point.y - vertexList[face[i]].y) / (vertexList[face[j]].y - vertexList[face[i]].y) + vertexList[face[i]].x)){
            float z1 = vertexList[face[i]].z;
            float z2 = vertexList[face[j]].z;
            z = z1 + (z2 - z1) * (point.x - vertexList[face[i]].x) / (vertexList[face[j]].x - vertexList[face[i]].x);
        }
    }
    return z;
}