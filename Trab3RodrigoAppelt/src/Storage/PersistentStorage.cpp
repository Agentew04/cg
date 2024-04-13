#include <fstream>
#include <algorithm>
#include <iterator>
#include <iostream>

#include "PersistentStorage.h"

// variaveis estaticas
std::string PersistentStorage::savePath = "";
bool PersistentStorage::initialized = false;
std::map<std::string, PersistentStorage::Container*> PersistentStorage::containers;

void PersistentStorage::init(){
    initialized = true;
}

void PersistentStorage::load(std::string path){
    init();

    // o arquivo vai estar formatado
    // estilo ini
    savePath = path;
    std::ifstream file(savePath);

    if(!file.is_open()){
        std::cout << "Nao consegui abrir o arquivo " << savePath << std::endl;
        file.close();
        return;
    }

    std::string currentContainer = "";
    std::string line;
    while(std::getline(file, line)){
        if(line.empty()){
            continue;
        }

        // tira espacos antes e dps
        line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
        line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());

        if(line[0] == '['){
            currentContainer = line.substr(1, line.size()-2);
            if(currentContainer.empty()){
                continue;
            }
            containers[currentContainer] = new Container();
            continue;
        }else{
            if(currentContainer.empty()){
                continue;
            }
            uint32_t div1Idx = line.find(':');
            uint32_t div2Idx = line.find('=');
            if(div1Idx == std::string::npos || div2Idx == std::string::npos){
                continue;
            }
            std::string key = line.substr(0, div1Idx);
            std::string type = line.substr(div1Idx+1, div2Idx-div1Idx-1);
            std::string value = line.substr(div2Idx+1);

            if(type == "int"){
                containers[currentContainer]->ints[key] = std::stoi(value);
            }else if(type == "float"){
                containers[currentContainer]->floats[key] = std::stof(value);
            }else if(type == "vec2"){
                uint32_t div3Idx = value.find(',');
                if(div3Idx == std::string::npos){
                    continue;
                }
                std::string x = value.substr(0, div3Idx);
                std::string y = value.substr(div3Idx+1);
                containers[currentContainer]->vec2s[key] = Vector2(std::stof(x), std::stof(y));
            }else if(type == "vec3"){
                uint32_t div3Idx = value.find(',');
                uint32_t div4Idx = value.find(',', div3Idx+1);
                if(div3Idx == std::string::npos || div4Idx == std::string::npos){
                    continue;
                }
                std::string x = value.substr(0, div3Idx);
                std::string y = value.substr(div3Idx+1, div4Idx-div3Idx-1);
                std::string z = value.substr(div4Idx+1);
                containers[currentContainer]->vec3s[key] = Vector3(std::stof(x), std::stof(y), std::stof(z));
            }else{
                continue;
            }
        }
    }
    file.close();
}

void PersistentStorage::save(){
    init();

    // sobrescreve o arquivo
    std::ofstream file(savePath);

    if(!file.is_open()){
        std::cout << "Nao consegui abrir o arquivo " << savePath << std::endl;
        file.close();
        return;
    }

    for(auto it = containers.begin(); it != containers.end(); it++){
        file << "[" << it->first << "]" << std::endl;
        for(auto it2 = it->second->ints.begin(); it2 != it->second->ints.end(); it2++){
            file << it2->first << ":int=" << it2->second << std::endl;
        }
        for(auto it2 = it->second->floats.begin(); it2 != it->second->floats.end(); it2++){
            file << it2->first << ":float=" << it2->second << std::endl;
        }
        for(auto it2 = it->second->vec2s.begin(); it2 != it->second->vec2s.end(); it2++){
            file << it2->first << ":vec2=" << it2->second.x << "," << it2->second.y << std::endl;
        }
        for(auto it2 = it->second->vec3s.begin(); it2 != it->second->vec3s.end(); it2++){
            file << it2->first << ":vec3=" << it2->second.x << "," << it2->second.y << "," << it2->second.z << std::endl;
        }
    }
}

void PersistentStorage::reset(){
    init();
    for(auto it = containers.begin(); it != containers.end(); it++){
        it->second->ints.clear();
        it->second->floats.clear();
        it->second->vec2s.clear();
        it->second->vec3s.clear();
        delete it->second;
    }
    containers.clear();
    save();
}