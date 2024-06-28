#include "Rasterizer.h"

#include <limits>

#include "Perspective.h"

float remap(float value, float low1, float high1, float low2, float high2){
    return low2 + (value - low1) * (high2 - low2) / (high1 - low1);
}

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
            for(size_t i = 0; i < face.vertices.size(); i++){
                if(camera.isOnFrustumCS(poly.vertexList[face.vertices[i]]) == false){
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
    Vector2 cameraPlanes = camera.getClipPanes();


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

                        float normalizedZ = remap(z, cameraPlanes.x, cameraPlanes.y, 0, 1);
                        // se o z é menor que o zbuffer
                        if(normalizedZ < zbuffer[depthBuffer->getOffset(x, y)]){
                            // atualiza o zbuffer
                            zbuffer[depthBuffer->getOffset(x, y)] = normalizedZ;

                            // calcula a cor do pixel
                            Vector3 normal = interpolateNormal(poly.vertexList, poly.normalList, face, Vector2(x, y));
                            //float intensity = std::max(0.0f, normal.dot(localLightDirection));
                            //Vector3 pixelcolor = color*intensity;
                            cbuf[colorBuffer->getOffset(x, y)] = remap(normal.x, -1, 1, 0, 1);
                            cbuf[colorBuffer->getOffset(x, y) + 1] = remap(normal.y, -1, 1, 0, 1);
                            cbuf[colorBuffer->getOffset(x, y) + 2] = remap(normal.z, -1, 1, 0, 1);
                        }
                    }
                }
            }
        }
    }

}

void Rasterizer::BoundBox(
    const std::vector<Vector3>& vertexList,
    const Primitive::Face& face,
    Vector2& p1,
    Vector2& p2){
    float minX = std::numeric_limits<float>::infinity();
    float minY = std::numeric_limits<float>::infinity();
    float maxX = -std::numeric_limits<float>::infinity();
    float maxY = -std::numeric_limits<float>::infinity();

    for(size_t i = 0; i < face.vertices.size(); i++){
        if(vertexList[face.vertices[i]].x < minX) minX = vertexList[face.vertices[i]].x;
        if(vertexList[face.vertices[i]].y < minY) minY = vertexList[face.vertices[i]].y;
        if(vertexList[face.vertices[i]].x > maxX) maxX = vertexList[face.vertices[i]].x;
        if(vertexList[face.vertices[i]].y > maxY) maxY = vertexList[face.vertices[i]].y;
    }
    p1 = Vector2(minX, minY);
    p2 = Vector2(maxX, maxY);
}

bool Rasterizer::isInside(
    const std::vector<Vector3>& vertexList,
    const Primitive::Face& face,
    Vector2 point){
    int n = face.vertices.size();
    bool inside = false;
    for(int i = 0, j = n - 1; i < n; j = i++){
        if(((vertexList[face.vertices[i]].y > point.y) != (vertexList[face.vertices[j]].y > point.y)) &&
            (point.x < (vertexList[face.vertices[j]].x - vertexList[face.vertices[i]].x) * (point.y - vertexList[face.vertices[i]].y) / (vertexList[face.vertices[j]].y - vertexList[face.vertices[i]].y) + vertexList[face.vertices[i]].x)){
            inside = !inside;
        }
    }
    return inside;
}

float Rasterizer::interpolateZ(
    const std::vector<Vector3>& vertexList,
    const Primitive::Face& face,
    Vector2 point){
    int n = face.vertices.size();
    if (n != 3) {
        std::cout << "InterpZ: Face nao triangular! Faces: " << n << std::endl;
        return 0.0f;
    }
    // coordenadas baricentrias, seja la oq isso for
    Vector3 p1 = vertexList[face.vertices[0]];
    Vector3 p2 = vertexList[face.vertices[1]];
    Vector3 p3 = vertexList[face.vertices[2]];
    Vector3 p(point.x, point.y, 0);
    double delta = 0.5 * fabs(p1.x*(p2.y-p3.y) + p2.x*(p3.y-p1.y) + p3.x*(p1.y-p2.y));
    double delta1 = 0.5 * fabs(p.x*(p2.y-p3.y) + p2.x*(p3.y-p.y) + p3.x*(p.y-p2.y));
    double delta2 = 0.5 * fabs(p1.x*(p.y-p3.y) + p.x*(p3.y-p1.y) + p3.x*(p1.y-p.y));
    double delta3 = 0.5 * fabs(p1.x*(p2.y-p.y) + p2.x*(p.y-p1.y) + p.x*(p1.y-p2.y));
    double z = (p1.z * delta1 + p2.z * delta2 + p3.z * delta3) / delta;
    return z;
}

Vector3 Rasterizer::interpolateNormal(
    const std::vector<Vector3>& vertexList,
    const std::vector<Vector3>& normalList,
    const Primitive::Face& face,
    Vector2 point){
    int n = face.vertices.size();
    if (n != 3) {
        std::cout << "InterpNorm: Face nao triangular! Faces: " << n << std::endl;
        return Vector3::zero();
    }
    Vector3 p1 = vertexList[face.vertices[0]];
    Vector3 p2 = vertexList[face.vertices[1]];
    Vector3 p3 = vertexList[face.vertices[2]];
    Vector3 n1 = normalList[face.normals[0]];
    Vector3 n2 = normalList[face.normals[1]];
    Vector3 n3 = normalList[face.normals[2]];
    Vector3 p(point.x, point.y, 0);
    double delta = 0.5 * fabs(p1.x*(p2.y-p3.y) + p2.x*(p3.y-p1.y) + p3.x*(p1.y-p2.y));
    double delta1 = 0.5 * fabs(p.x*(p2.y-p3.y) + p2.x*(p3.y-p.y) + p3.x*(p.y-p2.y));
    double delta2 = 0.5 * fabs(p1.x*(p.y-p3.y) + p.x*(p3.y-p1.y) + p3.x*(p1.y-p.y));
    double delta3 = 0.5 * fabs(p1.x*(p2.y-p.y) + p2.x*(p.y-p1.y) + p.x*(p1.y-p2.y));
    Vector3 norm = (n1 * delta1 + n2 * delta2 + n3 * delta3) / delta;
    return norm;
}
