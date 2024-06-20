#ifndef __PERSPECTIVE_H__
#define __PERSPECTIVE_H__

#include <vector>

#include "../Math/Vector2.h"
#include "../Math/Vector3.h"

/// @brief Classe para agrupar funcoes de transformacoes 3D.
class P3D {
public:
static Vector3 scaleX(Vector3 v, float s);
static Vector3 scaleY(Vector3 v, float s);
static Vector3 scaleZ(Vector3 v, float s);
static Vector3 scale(Vector3 v, Vector3 factor);
static std::vector<Vector3> scaleVector(std::vector<Vector3> v, Vector3 factor);

static Vector3 translateX(Vector3 v, float t);
static Vector3 translateY(Vector3 v, float t);
static Vector3 translateZ(Vector3 v, float t);
static Vector3 translate(Vector3 v, Vector3 t);
static std::vector<Vector3> translateVector(std::vector<Vector3> v, Vector3 t);


static Vector3 rotateX(Vector3 v, float angle);
static Vector3 rotateY(Vector3 v, float angle);
static Vector3 rotateZ(Vector3 v, float angle);
static Vector3 rotate(Vector3 v, Vector3 angles);
static std::vector<Vector3> rotateVector(std::vector<Vector3> v, Vector3 angles);

static Vector2 perspectiveProjection(Vector3 v, float d);
static std::vector<Vector2> perspectiveProjectionVector(std::vector<Vector3> v, float d);

static void sortFaces(const std::vector<Vector3>& vertices, std::vector<std::vector<int>>& faces);

};

#endif
