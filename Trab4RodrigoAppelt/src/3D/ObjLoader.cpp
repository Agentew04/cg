#include "ObjLoader.h"

#include <iostream>
#include <vector>
#include <fstream>

std::map<std::string, std::map<std::string, Model3D>> ObjLoader::models;

void ObjLoader::load(const std::string& filename, const std::string& id){
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open file " << filename << std::endl;
        return;
    }

    obj currentObj;
    int vertexIndex = 1;
    int objVertexStartIndex = 1;
    int normalIndex = 1;
    int objNormalStartIndex = 1;

    std::map<std::string, Model3D> models;

    std::string line;
    while (std::getline(file,line)){
        if(line[0] == '#') continue;
        if(line[0] == ' ') continue;
        if(line[0] == '\n') continue;

        if(line[0] == 's'){
            // nao suportamos smooth shading
            continue;
        }

        if(line[0] == 'o'){
            // submita o objeto anterior
            if(currentObj.vertices.size() > 0){
                // submit
                Model3D model;
                model.vertices = currentObj.vertices;
                model.faces = currentObj.faces;
                model.normals = currentObj.normals;
                model.faceNormals = currentObj.faceNormals;
                model.hasNormalData = currentObj.normals.size() > 0 && currentObj.faceNormals.size() > 0;
                models[currentObj.name] = model;
            }
            // comeca novo objeto
            currentObj.name = line.substr(2);
            currentObj.vertices.clear();
            currentObj.faces.clear();
            currentObj.normals.clear();
            currentObj.faceNormals.clear();
            objVertexStartIndex = vertexIndex;
            objNormalStartIndex = normalIndex;
        }

        if(line[0] == 'v'){
            if(line[1] == ' '){ // definicao de vertice
                float x, y, z;
                sscanf(line.c_str(), "v %f %f %f", &x, &y, &z);
                Vector3 vertex(x, y, z);
                currentObj.vertices.push_back(vertex);
                vertexIndex++;
                continue;
            }
            if(line[1] == 'n'){ // vetor normal
                float x, y, z;
                sscanf(line.c_str(), "vn %f %f %f", &x, &y, &z);
                Vector3 normal(x, y, z);
                currentObj.normals.push_back(normal);
                normalIndex++;
                continue;
            }
        }

        if(line[0] == 'f'){
            // support normals
            if(line.find("//") != std::string::npos){
                // we have normal data and dont have texture data
                int v1, v2, v3, n1, n2, n3;
                sscanf(line.c_str(), "f %d//%d %d//%d %d//%d", &v1, &n1, &v2, &n2, &v3, &n3);
                std::vector<int> face = {v1 - objVertexStartIndex, v2 - objVertexStartIndex, v3 - objVertexStartIndex};
                std::vector<int> faceNormal = {n1 - objNormalStartIndex, n2 - objNormalStartIndex, n3 - objNormalStartIndex};
                currentObj.faces.push_back(face);
                currentObj.faceNormals.push_back(faceNormal);
            } else {
                // no normal data on face
                std::vector<int> face;
                int v1, v2, v3;
                sscanf(line.c_str(), "f %d %d %d", &v1, &v2, &v3);
                face.push_back(v1 - objVertexStartIndex);
                face.push_back(v2 - objVertexStartIndex);
                face.push_back(v3 - objVertexStartIndex);
                currentObj.faces.push_back(face);
                continue;
            }
        }
    }

    // submita o ultimo objeto
    if(currentObj.vertices.size() > 0){
        // submit
        Model3D model;
        model.vertices = currentObj.vertices;
        model.faces = currentObj.faces;
        model.normals = currentObj.normals;
        model.faceNormals = currentObj.faceNormals;
        model.hasNormalData = currentObj.normals.size() > 0 && currentObj.faceNormals.size() > 0;
        models[currentObj.name] = model;
    }

    ObjLoader::models[id] = models;
    file.close();
}

void ObjLoader::free() {
    for(auto& mdl: models){
        for(auto& obj: mdl.second){
            obj.second.vertices.clear();
            obj.second.faces.clear();
        }
        mdl.second.clear();
    }
    models.clear();
}

Model3D* ObjLoader::get(const std::string& id, const std::string& objectName) {
    if(objectName != ""){
        return &models[id][objectName];
    }

    if(models[id].size() != 1) {
        std::cerr << "Warning: multiple objects in file. Fetching first" << id << std::endl;
    }
    return &models[id].begin()->second;
}

std::vector<std::string> ObjLoader::getModelNames(const std::string& id) {
    std::vector<std::string> names;
    for(auto& obj: models[id]){
        names.push_back(obj.first);
    }
    return names;
}

std::vector<std::string> ObjLoader::getIds() {
    std::vector<std::string> ids;
    for(auto& obj: models){
        ids.push_back(obj.first);
    }
    return ids;
}