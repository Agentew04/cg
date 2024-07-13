#include "TerrainBezier.h"

#include <random>
#include <vector>
#include <ctime>
#include <GL/glut.h>

#include "../Material.h"
#include "../Actor.h"
#include "../../Math/Vector3.h"

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
        float x = i % 4;
        float y = dis(gen);
        float z = i / 4;
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
}

float TerrainBezier::getHeightAt(float x, float z) const {
    // Ensure we have points and normals to render
    if (terrainPoints.empty() || terrainNormals.empty()) return 0.0f;

    auto actor = this->actor.lock();
    if(!actor){
        return 0.0f;
    }

    // normalize coordinates
    x = (x - actor->position.x) / actor->scale.x;
    z = (z - actor->position.z) / actor->scale.z;

    // evaluate the height at the given x and z coordinates
    float step = 1.0f / (resolution - 1);
    float u = x / step;
    float v = z / step;
    Vector3 point = evaluateBezierSurface(controlPoints, 0, 1, 4, u, v);
    return point.y;
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
    if(textureId != 0){
        //render_with_texture();
    } else {
        render_without_texture();
    }
    glPopMatrix();
}

void TerrainBezier::render_without_texture(){

    // Ensure we have points and normals to render
    if (terrainPoints.empty() || terrainNormals.empty()) return;

    Material terrainMaterial;
    terrainMaterial.setAmbient(0.0f, 0.3f, 0.0f, 1.0f);
    terrainMaterial.setDiffuse(0.1f, 0.8f, 0.1f, 1.0f);
    terrainMaterial.setSpecular(1.0f, 1.0f, 1.0f, 1.0f);
    terrainMaterial.shininess = 10;
    terrainMaterial.use();

    glBegin(GL_TRIANGLES);
    for(int x=0; x<resolution-1; x++){
        for(int z=0; z<resolution-1; z++){
            int linear = x * resolution + z;
            auto actual = terrainPoints[linear];
            auto actualNormal = terrainNormals[linear];
            auto right = terrainPoints[linear + 1];
            auto rightNormal = terrainNormals[linear + 1];
            auto up = terrainPoints[linear + resolution];
            auto upNormal = terrainNormals[linear + resolution];
            auto diag = terrainPoints[linear + resolution + 1];
            auto diagNormal = terrainNormals[linear + resolution + 1];

            glNormal3f(actualNormal.x, actualNormal.y, actualNormal.z);
            glVertex3f(actual.x, actual.y, actual.z);
            glNormal3f(rightNormal.x, rightNormal.y, rightNormal.z);
            glVertex3f(right.x, right.y, right.z);
            glNormal3f(upNormal.x, upNormal.y, upNormal.z);
            glVertex3f(up.x, up.y, up.z);

            glNormal3f(rightNormal.x, rightNormal.y, rightNormal.z);
            glVertex3f(right.x, right.y, right.z);
            glNormal3f(diagNormal.x, diagNormal.y, diagNormal.z);
            glVertex3f(diag.x, diag.y, diag.z);
            glNormal3f(upNormal.x, upNormal.y, upNormal.z);
            glVertex3f(up.x, up.y, up.z);
        }
    }
    glEnd();
    
}
