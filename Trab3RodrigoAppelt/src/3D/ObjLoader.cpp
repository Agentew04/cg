#include "ObjLoader.h"

#include <iostream>
#include <vector>
#include <fstream>

std::map<std::string, ObjFile*> ObjLoader::objFiles;

void ObjLoader::load(const std::string& filename, const std::string& id) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open file " << filename << std::endl;
        return;
    }

    std::vector<Vector3> vertices;
    std::vector<std::vector<int>> faces;

    std::string line;
    while (std::getline(file, line)) {
        if(line[0] == '#') continue; // Ignore comments
        if(line[0] == ' ') continue;
        if(line[0] == '\n') continue;

        if (line[0] == 'v' && line[1] == ' ') {
            float x, y, z;
            sscanf(line.c_str(), "v %f %f %f", &x, &y, &z);
            Vector3 vertex(x, y, z);
            vertices.push_back(vertex);
            continue;
        }
        if (line[0] == 'f' && line[1] == ' ') {
            std::vector<int> face;
            int v1, v2, v3;
            sscanf(line.c_str(), "f %d %d %d", &v1, &v2, &v3);
            face.push_back(v1);
            face.push_back(v2);
            face.push_back(v3);
            faces.push_back(face);
            continue;
        }
    }
    ObjFile *obj = new ObjFile();
    obj->vertices = vertices;
    obj->faces = faces;
    objFiles[id] = obj;
    file.close();
    std::cout << "Loaded " << filename << " as \"" << id << "\". "
        << std::to_string(obj->vertices.size()) << " vertices and " 
        << std::to_string(obj->faces.size()) << " faces." << std::endl;
}

void ObjLoader::free() {
    for (auto& obj : objFiles) {
        delete obj.second;
    }
    objFiles.clear();
}

ObjFile* ObjLoader::get(const std::string& id) {
    return objFiles[id];
}
