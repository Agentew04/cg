#include "MeshImporter.h"

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <optional>
#include <sstream>
#include <fstream>
#include <algorithm>

#include "Material.h"
#include "Mesh.h"

enum class LineType{
    COMMENT,
    OBJECT,
    VERTEX,
    NORMAL,
    UV,
    FACE,
    MTL_LIB,
    USE_MATERIAL,
    UNKNOWN,
    NEW_MATERIAL,
    K_AMBIENT,
    K_DIFFUSE,
    K_SPECULAR,
    K_EMISSIVE,
    N_SPECULAR
};

static inline void trim(std::string &s);
static inline bool starts_with(const std::string &s, const std::string &prefix);
static inline bool canIngore(const std::string &s);
static inline LineType getLineType(const std::string &s);
static inline Vector3 parseVertex(const std::string &s);
static inline Vector3 parseNormal(const std::string &s);
static inline Vector2 parseUv(const std::string &s);
static inline std::string parseMtlLib(const std::string &s);
static inline void parseFace(const std::string &s, Engine::Mesh::Face &face);

static inline std::string parseMaterialName(const std::string &s);
static inline Vector3 parseAmbientColor(const std::string &s);
static inline Vector3 parseDiffuseColor(const std::string &s);
static inline Vector3 parseSpecularColor(const std::string &s);
static inline float parseSpecularExponent(const std::string &s);



using namespace Engine;

std::shared_ptr<Engine::Mesh> MeshImporter::loadMesh(std::string path){
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Could not open file " << path << std::endl;
        return nullptr;
    }
    std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

    std::vector<Material> materials;
    std::map<std::string, int> materialMapping;
    std::string currentMaterial = "";

    std::string line;
    while(std::getline(file,line)){
        trim(line);

        if(canIngore(line)){
            continue;
        }


        LineType type = getLineType(line);
        switch(type){
            case LineType::MTL_LIB: {
                auto dir = path.substr(0, path.find_last_of("/\\") + 1);
                auto mtlPath = dir + parseMtlLib(line);
                auto mats = loadMaterials(mtlPath);
                for(auto &mat : mats){
                    materialMapping[mat.first] = mesh->materials.size();
                    mesh->materials.push_back(mat.second);
                }
                break;
            }
            case LineType::OBJECT: {
                // soh suportamos arquivos com um objeto por que muita mao
                break;
            }
            case LineType::VERTEX:
                mesh->vertexList.push_back(parseVertex(line));
                break;
            case LineType::NORMAL:
                mesh->normalList.push_back(parseNormal(line));
                break;
            case LineType::UV:
                mesh->uvList.push_back(parseUv(line));
                break;
            case LineType::USE_MATERIAL:
                currentMaterial = parseMtlLib(line);
                break;
            case LineType::FACE: {
                Mesh::Face face;
                parseFace(line, face);
                face.material = materialMapping[currentMaterial];
                mesh->faceList.push_back(face);
                break;
            }
            case LineType::UNKNOWN:
                break;
        }
    }

    // convert material name to int indexes
    return mesh;
}

std::map<std::string, Engine::Material> MeshImporter::loadMaterials(std::string path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Could not open file " << path << std::endl;
        return std::map<std::string, Material>();
    }

    std::map<std::string, Material> materials;

    std::optional<Material> currentMaterial = std::nullopt;
    std::string currentMaterialName = "";

    std::string line;
    while(std::getline(file,line)){
        trim(line);

        if(canIngore(line)){
            continue;
        }

        LineType type = getLineType(line);
        switch(type){
            case LineType::NEW_MATERIAL: {
                if(currentMaterial.has_value()){
                    materials[currentMaterialName] = currentMaterial.value();
                }
                currentMaterial = Material();
                currentMaterialName = parseMaterialName(line);
                break;
            }
            case LineType::K_AMBIENT: {
                auto amb = parseAmbientColor(line);
                currentMaterial->ambient[0] = amb.x;
                currentMaterial->ambient[1] = amb.y;
                currentMaterial->ambient[2] = amb.z;
                currentMaterial->ambient[3] = 1.0f;
                break;
            }
            case LineType::K_DIFFUSE: {
                auto diff = parseDiffuseColor(line);
                currentMaterial->diffuse[0] = diff.x;
                currentMaterial->diffuse[1] = diff.y;
                currentMaterial->diffuse[2] = diff.z;
                currentMaterial->diffuse[3] = 1.0f;
                break;
            }
            case LineType::K_SPECULAR: {
                auto spec = parseSpecularColor(line);
                currentMaterial->specular[0] = spec.x;
                currentMaterial->specular[1] = spec.y;
                currentMaterial->specular[2] = spec.z;
                currentMaterial->specular[3] = 1.0f;
                break;
            }
            case LineType::N_SPECULAR: {
                currentMaterial->shininess = parseSpecularExponent(line);
                break;
            }
            case LineType::UNKNOWN:
                break;
        }

        // add last
        if(currentMaterial.has_value()){
            materials[currentMaterialName] = currentMaterial.value();
        }
    }
    return materials;
}

static inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return !std::isspace(ch);
    }));
}
static inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return !std::isspace(ch);
    }).base(), s.end());
}
static inline void trim(std::string &s) {
    ltrim(s);
    rtrim(s);
}

static inline bool canIngore(const std::string &s){
    return s.empty() || s[0] == '#' || s[0] == 's';
}

static inline LineType getLineType(const std::string &s){
    if(s[0] == 'v'){
        if(s[1] == 'n'){
            return LineType::NORMAL;
        }
        if(s[1] == 't'){
            return LineType::UV;
        }
        return LineType::VERTEX;
    }
    if(s[0] == 'f'){
        return LineType::FACE;
    }
    if(s[0] == '#'){
        return LineType::COMMENT;
    }
    if(starts_with(s, "newmtl")){
        return LineType::NEW_MATERIAL;
    }
    if(starts_with(s, "Ka")){
        return LineType::K_AMBIENT;
    }
    if(starts_with(s, "Kd")){
        return LineType::K_DIFFUSE;
    }
    if(starts_with(s, "Ks")){
        return LineType::K_SPECULAR;
    }
    if(starts_with(s, "Ns")){
        return LineType::N_SPECULAR;
    }
    if(starts_with(s, "mtllib")){
        return LineType::MTL_LIB;
    }
    if(starts_with(s, "usemtl")){
        return LineType::USE_MATERIAL;
    }

    return LineType::UNKNOWN;
}

static inline Vector3 parseVertex(const std::string &s){
    float x, y, z;
    sscanf(s.c_str(), "v %f %f %f", &x, &y, &z);
    return Vector3(x, y, z);
}

static inline Vector3 parseNormal(const std::string &s){
    float x, y, z;
    sscanf(s.c_str(), "vn %f %f %f", &x, &y, &z);
    return Vector3(x, y, z);
}

static inline Vector2 parseUv(const std::string &s){
    float x, y;
    sscanf(s.c_str(), "vt %f %f", &x, &y);
    return Vector2(x, y);
}

static inline std::string parseMaterialName(const std::string &s){
    return s.substr(7);
}

static inline bool starts_with(const std::string &s, const std::string &prefix){
    return s.rfind(prefix, 0) == 0;
}

static inline Vector3 parseAmbientColor(const std::string &s) {
    float r, g, b;
    int read = sscanf(s.c_str(), "Ka %f %f %f", &r, &g, &b);
    if (read != 3) {
        std::cerr << "Error parsing ambient color: " << s << ". Expected 3 values to read." << std::endl;
        return Vector3(0, 0, 0);
    }
    return Vector3(r, g, b);
}

static inline Vector3 parseDiffuseColor(const std::string &s) {
    float r, g, b;
    int read = sscanf(s.c_str(), "Kd %f %f %f", &r, &g, &b);
    if (read != 3) {
        std::cerr << "Error parsing diffuse color: " << s << ". Expected 3 values to read." << std::endl;
        return Vector3(0, 0, 0);
    }
    return Vector3(r, g, b);
}

static inline Vector3 parseSpecularColor(const std::string &s){
    float r, g, b;
    int read = sscanf(s.c_str(), "Ks %f %f %f", &r, &g, &b);
    if (read != 3) {
        std::cerr << "Error parsing specular color: " << s << ". Expected 3 values to read." << std::endl;
        return Vector3(0, 0, 0);
    }
    return Vector3(r, g, b);
}

static inline float parseSpecularExponent(const std::string &s){
    float n;
    int read = sscanf(s.c_str(), "Ns %f", &n);
    if (read != 1) {
        std::cerr << "Error parsing specular exponent: " << s << ". Expected 1 value to read." << std::endl;
        return 0;
    }
    return n;
}

static inline std::string parseMtlLib(const std::string &s){
    return s.substr(7);
}

static inline void parseFace(const std::string &s, Mesh::Face &face){
    std::istringstream iss(s);
    std::string token;
    // Skip the 'f' part
    iss >> token;

    while (iss >> token) {
        std::istringstream vertexData(token);
        std::string index;
        int vIdx = -1, vtIdx = -1, vnIdx = -1;

        std::getline(vertexData, index, '/');
        if (!index.empty()) vIdx = std::stoi(index) - 1;
        if (std::getline(vertexData, index, '/')) {
            if (!index.empty()) vtIdx = std::stoi(index) - 1;
            if (std::getline(vertexData, index, '/')) {
                if (!index.empty()) vnIdx = std::stoi(index) - 1;
            }
        }

        face.vertices.push_back(vIdx);
        if (vtIdx != -1) face.uv.push_back(vtIdx);
        if (vnIdx != -1) face.normals.push_back(vnIdx);
    }

}