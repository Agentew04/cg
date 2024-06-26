#include "Perspective.h"

#include <vector>
#include <algorithm>
#include "../Math/Vector3.h"

Vector3 P3D::scaleX(Vector3 v, float s){
    return Vector3(v.x * s, v.y, v.z);
}

Vector3 P3D::scaleY(Vector3 v, float s){
    return Vector3(v.x, v.y * s, v.z);
}

Vector3 P3D::scaleZ(Vector3 v, float s){
    return Vector3(v.x, v.y, v.z * s);
}

Vector3 P3D::scale(Vector3 v, Vector3 factor){
    return Vector3(v.x * factor.x, v.y * factor.y, v.z * factor.z);
}

std::vector<Vector3> P3D::scaleVector(std::vector<Vector3> v, Vector3 factor){
    std::vector<Vector3> result;
    for(Vector3 vec : v){
        result.push_back(scale(vec, factor));
    }
    return result;
}

std::vector<Vector3> P3D::scaleVectorX(std::vector<Vector3> v, float factor){
    std::vector<Vector3> result;
    for(Vector3 vec : v){
        result.push_back(scaleX(vec, factor));
    }
    return result;
}

std::vector<Vector3> P3D::scaleVectorY(std::vector<Vector3> v, float factor){
    std::vector<Vector3> result;
    for(Vector3 vec : v){
        result.push_back(scaleY(vec, factor));
    }
    return result;
}

std::vector<Vector3> P3D::scaleVectorZ(std::vector<Vector3> v, float factor){
    std::vector<Vector3> result;
    for(Vector3 vec : v){
        result.push_back(scaleZ(vec, factor));
    }
    return result;
}

Vector3 P3D::translateX(Vector3 v, float t){
    return Vector3(v.x + t, v.y, v.z);
}

Vector3 P3D::translateY(Vector3 v, float t){
    return Vector3(v.x, v.y + t, v.z);
}

Vector3 P3D::translateZ(Vector3 v, float t){
    return Vector3(v.x, v.y, v.z + t);
}

Vector3 P3D::translate(Vector3 v, Vector3 t){
    return Vector3(v.x + t.x, v.y + t.y, v.z + t.z);
}

std::vector<Vector3> P3D::translateVector(std::vector<Vector3> v, Vector3 t){
    std::vector<Vector3> result;
    for(Vector3 vec : v){
        result.push_back(translate(vec, t));
    }
    return result;
}

std::vector<Vector3> P3D::translateVectorX(std::vector<Vector3> v, float t){
    std::vector<Vector3> result;
    for(Vector3 vec : v){
        result.push_back(translateX(vec, t));
    }
    return result;
}

std::vector<Vector3> P3D::translateVectorY(std::vector<Vector3> v, float t){
    std::vector<Vector3> result;
    for(Vector3 vec : v){
        result.push_back(translateY(vec, t));
    }
    return result;
}

std::vector<Vector3> P3D::translateVectorZ(std::vector<Vector3> v, float t){
    std::vector<Vector3> result;
    for(Vector3 vec : v){
        result.push_back(translateZ(vec, t));
    }
    return result;
}

Vector3 P3D::rotateX(Vector3 v, float angle){
    float s = sin(angle);
    float c = cos(angle);
    return Vector3(v.x, v.y * c - v.z * s, v.y * s + v.z * c);
}

Vector3 P3D::rotateY(Vector3 v, float angle){
    float s = sin(angle);
    float c = cos(angle);
    return Vector3(v.x * c + v.z * s, v.y, -v.x * s + v.z * c);
}

Vector3 P3D::rotateZ(Vector3 v, float angle){
    float s = sin(angle);
    float c = cos(angle);
    return Vector3(v.x * c - v.y * s, v.x * s + v.y * c, v.z);
}

Vector3 P3D::rotate(Vector3 v, Vector3 angles){
    return rotateZ(rotateY(rotateX(v, angles.x), angles.y), angles.z);
}

std::vector<Vector3> P3D::rotateVector(std::vector<Vector3> v, Vector3 angles){
    std::vector<Vector3> result;
    for(Vector3 vec : v){
        result.push_back(rotate(vec, angles));
    }
    return result;
}

std::vector<Vector3> P3D::rotateVectorX(std::vector<Vector3> v, float angles){
    std::vector<Vector3> result;
    for(Vector3 vec : v){
        result.push_back(rotateX(vec, angles));
    }
    return result;
}

std::vector<Vector3> P3D::rotateVectorY(std::vector<Vector3> v, float angles){
    std::vector<Vector3> result;
    for(Vector3 vec : v){
        result.push_back(rotateY(vec, angles));
    }
    return result;
}

std::vector<Vector3> P3D::rotateVectorZ(std::vector<Vector3> v, float angles){
    std::vector<Vector3> result;
    for(Vector3 vec : v){
        result.push_back(rotateZ(vec, angles));
    }
    return result;
}

Vector2 P3D::perspectiveProjection(Vector3 v, float d){
    return Vector2(v.x * d / v.z, v.y * d / v.z);
}

std::vector<Vector2> P3D::perspectiveProjectionVector(std::vector<Vector3> v, float d){
    std::vector<Vector2> result;
    for(Vector3 vec : v){
        result.push_back(perspectiveProjection(vec, d));
    }
    return result;
}


void P3D::sortFaces(const std::vector<Vector3>& vertices, std::vector<std::vector<int>>& faces){
    auto zDistance = [&vertices](const std::vector<int>& face) {
        float sumZ = 0.0f;
        for (int index : face) {
            sumZ += vertices[index].z;
        }
        return sumZ / face.size();
    };


    // Sort the faces based on the average Z-coordinate
    std::sort(faces.begin(), faces.end(), [&zDistance](const std::vector<int>& face1, const std::vector<int>& face2) {
        return zDistance(face1) > zDistance(face2);
    });
}


std::vector<Vector3> P3D::rotateVectorAxis(std::vector<Vector3> v, Vector3 axis, float angle){
    std::vector<Vector3> out;

    axis.normalize();
    for(auto &point : v){
        float dot = axis.dot(point);
        Vector3 cross = axis.cross(point);

        float x = point.x * cos(angle) + cross.x * sin(angle) + axis.x * dot * (1 - cos(angle));
        float y = point.y * cos(angle) + cross.y * sin(angle) + axis.y * dot * (1 - cos(angle));
        float z = point.z * cos(angle) + cross.z * sin(angle) + axis.z * dot * (1 - cos(angle));

        out.push_back(Vector3(x, y, z));
    }
    return out;
}