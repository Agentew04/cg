#include "TerrainBezier.h"

#include <random>
#include <vector>
#include <ctime>
#include <chrono>
#include <GL/glut.h>

#include "../Material.h"
#include "../Actor.h"
#include "../../Math/Vector3.h"
#include "../../2D/lodepng.h"

#ifdef ERROR
#undef ERROR
#endif

using namespace Engine::Components;


// Helper function to calculate a point on a cubic Bezier curve
Vector3 evaluateBezierCurve(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t) {
    float u = 1.0f - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;

    Vector3 point = p0 * uuu; // (1-t)^3 * p0
    point += p1 * (3 * uu * t); // 3 * (1-t)^2 * t * p1
    point += p2 * (3 * u * tt); // 3 * (1-t) * t^2 * p2
    point += p3 * ttt;        // t^3 * p3

    return point;
}

// Helper function to calculate the derivative of a cubic Bezier curve
Vector3 evaluateBezierCurveDerivative(const Vector3& p0, const Vector3& p1, const Vector3& p2, const Vector3& p3, float t) {
    float u = 1.0f - t;
    float tt = t * t;
    float uu = u * u;

    Vector3 point = p0 * (-3 * uu); // -3 * (1-t)^2 * p0
    point += p1 * (3 * (uu - 2 * u * t)); // 3 * (1-t) * (1-3t) * p1
    point += p2 * (3 * (2 * t - tt)); // 3 * t * (2 - 3t) * p2
    point += p3 * (3 * tt); // 3 * t^2 * p3

    return point;
}

// Helper function to evaluate a point on a Bezier surface
Vector3 evaluateBezierSurface(const std::vector<Vector3>& controlPoints, int patchIndex, int patches, int controlPointsPerPatch, float u, float v) {
    std::vector<Vector3> uCurve(4);
    int rowIndex = patchIndex / patches;
    int colIndex = patchIndex % patches;

    // Get control points for the current patch
    for (int i = 0; i < 4; ++i) {
        int index = (rowIndex * controlPointsPerPatch + i) * (patches * controlPointsPerPatch) + (colIndex * controlPointsPerPatch);
        uCurve[i] = evaluateBezierCurve(controlPoints[index], controlPoints[index + 1], controlPoints[index + 2], controlPoints[index + 3], u);
    }

    // Evaluate the Bezier surface at (u, v)
    return evaluateBezierCurve(uCurve[0], uCurve[1], uCurve[2], uCurve[3], v);
}

// Helper function to evaluate the derivative of a Bezier surface in the u direction
Vector3 evaluateBezierSurfaceDu(const std::vector<Vector3>& controlPoints, int patchIndex, int patches, int controlPointsPerPatch, float u, float v) {
    std::vector<Vector3> uCurve(4);
    int rowIndex = patchIndex / patches;
    int colIndex = patchIndex % patches;

    // Get control points for the current patch
    for (int i = 0; i < 4; ++i) {
        int index = (rowIndex * controlPointsPerPatch + i) * (patches * controlPointsPerPatch) + (colIndex * controlPointsPerPatch);
        uCurve[i] = evaluateBezierCurve(controlPoints[index], controlPoints[index + 1], controlPoints[index + 2], controlPoints[index + 3], u);
    }

    // Evaluate the derivative of the Bezier surface in the u direction at (u, v)
    return evaluateBezierCurveDerivative(uCurve[0], uCurve[1], uCurve[2], uCurve[3], v);
}

// Helper function to evaluate the derivative of a Bezier surface in the v direction
Vector3 evaluateBezierSurfaceDv(const std::vector<Vector3>& controlPoints, int patchIndex, int patches, int controlPointsPerPatch, float u, float v) {
        std::vector<Vector3> vCurve(4);
        int rowIndex = patchIndex / patches;
        int colIndex = patchIndex % patches;

        // Get control points for the current patch
        for (int i = 0; i < 4; ++i) {
            int index = (rowIndex * controlPointsPerPatch) * (patches * controlPointsPerPatch) + (colIndex * controlPointsPerPatch + i);
            vCurve[i] = evaluateBezierCurve(controlPoints[index], controlPoints[index + patches * controlPointsPerPatch], controlPoints[index + 2 * patches * controlPointsPerPatch], controlPoints[index + 3 * patches * controlPointsPerPatch], v);
        }

        // Evaluate the derivative of the Bezier surface in the v direction at (u, v)
        return evaluateBezierCurveDerivative(vCurve[0], vCurve[1], vCurve[2], vCurve[3], u);
    }


void TerrainBezier::Start(){
    if(initialized) {
        log(LogLevel::WARNING, "Terreno jah inicializado. Pulando");
        return;
    }
    std::mt19937 gen(time(nullptr));
    std::uniform_real_distribution<> dis(0.0, 10.0);

    // Clear the existing terrain points and normals
    terrainPoints.clear();
    terrainNormals.clear();

    // Number of patches in the terrain
    int totalControlPoints = 4 * 4;

    // Create a local array for the control points
    controlPoints.clear();
    controlPoints.reserve(totalControlPoints);

    // Generate random control points with random heights
    for (int i = 0; i < totalControlPoints; ++i) {
        float x = (i % 4)/3.0;
        float y = dis(gen);
        float z = i / 4/3.0;
        controlPoints.emplace_back(x, y, z);
    }

    // Generate tessellated points and their normals
    float step = 1.0f / (resolution - 1);
    for (int i = 0; i < resolution; ++i) {
        for (int j = 0; j < resolution; ++j) {
            float u = i * step;
            float v = j * step;
            Vector3 point = evaluateBezierSurface(controlPoints, 0, 1, 4, u, v);
            Vector3 du = evaluateBezierSurfaceDu(controlPoints, 0, 1, 4, u, v);
            Vector3 dv = evaluateBezierSurfaceDv(controlPoints, 0, 1, 4, u, v);
            Vector3 normal = du.cross(dv).normalized(); // Calculate the normal vector
            terrainPoints.push_back(point);
            terrainNormals.push_back(normal); // Store the normal vector
        }
    }

    for(auto& pt:terrainPoints){
        if(pt.x > 1.0001 || pt.z > 1.0001 || pt.x < 0 || pt.z < 0){
            log(LogLevel::ERROR, "Ponto fora do terreno! "+ std::to_string(pt.x)+", "+std::to_string(pt.z)+")");
        }
    }
    loadTexture();
    initialized = true;
}

float TerrainBezier::getHeightAt(float x, float z) const {
    // Ensure we have points and normals to render
    if (terrainPoints.empty() || terrainNormals.empty()) {
        log(LogLevel::ERROR, "Nao haviam valores nos vetores. terrainPoints.size="+std::to_string(terrainPoints.size())
            +"; terrainNormals.size="+std::to_string(terrainNormals.size()));
        return 0.0f;
    }

    auto actor = this->actor.lock();
    if(!actor){
        log(LogLevel::ERROR, "Nao consegui dar lock em ator");
        return 0.0f;
    }

    // normalize coordinates
    x += actor->scale.x * 0.5f;
    z += actor->scale.z * 0.5f;
    x /= actor->scale.x;
    z /= actor->scale.z;
    // x e z estao no intervalo [0,1] agora

    Vector3 point = evaluateBezierSurface(controlPoints, 0, 1, 4, x, z);
    return point.y * actor->scale.y + actor->position.y;
}

void TerrainBezier::loadTexture(){
    std::vector<unsigned char> image; // The raw pixels
    unsigned width, height;
    auto start = std::chrono::high_resolution_clock::now();
    unsigned error = lodepng::decode(image, width, height, ".\\Trab5RodrigoAppelt\\assets\\images\\terrain.png");
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    if(error){
        log(LogLevel::ERROR, "LodePNG Error: " + std::string(lodepng_error_text(error)));
        return;
    }

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        GL_RGB,
        width, // bmp.getWidth(),
        height,// bmp.getHeight(),
        0,
        GL_RGBA,
        GL_UNSIGNED_BYTE,
        image.data()// bmp.getImage()
    );

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //std::cout << "Texture ("<<textureId <<") loaded in " << duration.count() << " seconds" << std::endl;
}


void TerrainBezier::Destroy(){
    if(textureId != 0){
        glDeleteTextures(1, &textureId);
    }
    terrainNormals.clear();
    terrainPoints.clear();
}

void TerrainBezier::Render(){
    glPushMatrix();
    render_internal(textureId != 0);
    glPopMatrix();
}

void TerrainBezier::render_internal(bool useTexture){

    // Ensure we have points and normals to render
    if (terrainPoints.empty() || terrainNormals.empty()) return;

    if(useTexture && textureId != 0){
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, textureId);
        Material terrainMaterial;
        terrainMaterial.setAmbient(1.0f, 1.0f, 1.0f, 1.0f);
        terrainMaterial.setDiffuse(1.0f, 1.0f, 1.0f, 1.0f);
        terrainMaterial.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
        terrainMaterial.shininess = 0;
        terrainMaterial.use();
    }else{
        Material terrainMaterial;
        terrainMaterial.setAmbient(0.0f, 0.3f, 0.0f, 1.0f);
        terrainMaterial.setDiffuse(0.1f, 0.8f, 0.1f, 1.0f);
        terrainMaterial.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
        terrainMaterial.shininess = 10;
        terrainMaterial.use();
    }

    glBegin(GL_TRIANGLES);
    for(int x=0; x<resolution-1; x++){
        for(int z=0; z<resolution-1; z++){
            int linear = x * resolution + z;
            auto actual = terrainPoints[linear];
            auto actualNormal = terrainNormals[linear];
            auto actualUv = Vector2(x/(float)resolution, z/(float)resolution);
            auto right = terrainPoints[linear + 1];
            auto rightNormal = terrainNormals[linear + 1];
            auto rightUv = Vector2((x+1)/(float)resolution, z/(float)resolution);
            auto up = terrainPoints[linear + resolution];
            auto upNormal = terrainNormals[linear + resolution];
            auto upUv = Vector2(x/(float)resolution, (z+1)/(float)resolution);
            auto diag = terrainPoints[linear + resolution + 1];
            auto diagNormal = terrainNormals[linear + resolution + 1];
            auto diagUv = Vector2((x+1)/(float)resolution, (z+1)/(float)resolution);

            glNormal3f(actualNormal.x, actualNormal.y, actualNormal.z);
            glTexCoord2f(actualUv.x, actualUv.y);
            glVertex3f(actual.x, actual.y, actual.z);

            glNormal3f(rightNormal.x, rightNormal.y, rightNormal.z);
            glTexCoord2f(upUv.x, upUv.y);
            glVertex3f(right.x, right.y, right.z);

            glNormal3f(upNormal.x, upNormal.y, upNormal.z);
            glTexCoord2f(rightUv.x, rightUv.y);
            glVertex3f(up.x, up.y, up.z);

            glNormal3f(rightNormal.x, rightNormal.y, rightNormal.z);
            glTexCoord2f(upUv.x, upUv.y);
            glVertex3f(right.x, right.y, right.z);

            glNormal3f(diagNormal.x, diagNormal.y, diagNormal.z);
            glTexCoord2f(diagUv.x, diagUv.y);
            glVertex3f(diag.x, diag.y, diag.z);

            glNormal3f(upNormal.x, upNormal.y, upNormal.z);
            glTexCoord2f(rightUv.x, rightUv.y);
            glVertex3f(up.x, up.y, up.z);
        }
    }
    glEnd();
    if(useTexture){
        glDisable(GL_TEXTURE_2D);
    }

}
