#include <fstream>
#include <algorithm>
#include <iterator>
#include <iostream>
#include <sstream>
#include <iomanip>

#include "PersistentStorage.h"

// variaveis estaticas
std::string PersistentStorage::savePath = "";
std::map<std::string, PersistentStorage::Container*> PersistentStorage::containers;

void PersistentStorage::load(std::string path){
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
            uint32_t div2Idx = line.find('/');
            uint32_t div3Idx = line.find('=');

            if(div1Idx == std::string::npos || div2Idx == std::string::npos || div3Idx == std::string::npos){
                continue;
            }

            std::string key = line.substr(0, div1Idx);
            int typeSize = std::stoi(line.substr(div1Idx+1, div2Idx-div1Idx-1));
            uint32_t metadata = std::stoull(line.substr(div2Idx+1, div3Idx-div2Idx-1), nullptr, 16);
            std::string data = line.substr(div3Idx+1);
            

            void* ptr = new uint8_t[typeSize];
            containers[currentContainer]->heap[key] = ptr;
            containers[currentContainer]->heapSizes[ptr] = typeSize;
            containers[currentContainer]->metadatas[ptr] = metadata;

            for(int i = 0; i < typeSize; i++){
                std::string byte = data.substr(i*2, 2);
                ((uint8_t*)ptr)[i] = std::stoi(byte, nullptr, 16);
            }
        }
    }
    file.close();
}


void PersistentStorage::save(){
    // sobrescreve o arquivo
    std::ofstream file(savePath);

    if(!file.is_open()){
        std::cout << "Nao consegui abrir o arquivo " << savePath << std::endl;
        file.close();
        return;
    }

    for(auto it = containers.begin(); it != containers.end(); it++){
        file << "[" << it->first << "]" << std::endl;
        for(auto kvp : it->second->heap){
            std::string key = kvp.first;
            int size = it->second->heapSizes[kvp.second];
            uint32_t metadata = it->second->metadatas[kvp.second];

            if(kvp.second == nullptr){
                continue;
            }
            if(size == 0){
                continue;
            }

            file << kvp.first << ":" << size << "/";
            file << std::hex << std::setw(2) << std::setfill('0')
                << it->second->metadatas[kvp.second]
                << std::dec << "=";

            for(int i = 0; i < size; i++){
                file << std::hex << std::setw(2) << std::setfill('0') << (int)((uint8_t*)kvp.second)[i];
            }
            file << std::dec << std::endl;
        }
    }
}

void PersistentStorage::reset(){
    for(auto it = containers.begin(); it != containers.end(); it++){
        delete it->second;
    }
    containers.clear();
    save();
}

PersistentStorage::Container::~Container(){
    for(auto it = heap.begin(); it != heap.end(); it++){
        delete it->second;
    }
    heap.clear();
    heapSizes.clear();
}

bool PersistentStorage::isValidIdentifier(std::string container, std::string key){
    for(char c : container){
        if(c == '[' || c == ']' || c == ':' || c == '=' || c == '/'
            || !isalnum(c)){
            return false;
        }
    }

    for(char c : key){
        if(c == '[' || c == ']' || c == ':' || c == '=' || c == '/'
            || !isalnum(c)){
            return false;
        }
    }
    return true;
}



