#include "PersistentStorage.h"

void PersistentStorage::getVec3(std::string container, std::string key, Vector3 *value){
    init();
    if(containers.find(container) == containers.end()){
        return;
    }
    if(containers[container]->vec3s.find(key) == containers[container]->vec3s.end()){
        return;
    }
    *value = containers[container]->vec3s[key];
}

void PersistentStorage::setVec3(std::string container, std::string key, Vector3 value){
    init();
    if(containers.find(container) == containers.end()){
        containers[container] = new Container();
    }
    containers[container]->vec3s[key] = value;
    save();
}

bool PersistentStorage::hasVec3(std::string container, std::string key){
    init();
    if(containers.find(container) == containers.end()){
        return false;
    }
    if(containers[container]->vec3s.find(key) == containers[container]->vec3s.end()){
        return false;
    }
    return true;
}

void PersistentStorage::removeVec3(std::string container, std::string key){
    init();
    if(containers.find(container) == containers.end()){
        return;
    }
    if(containers[container]->vec3s.find(key) == containers[container]->vec3s.end()){
        return;
    }
    containers[container]->vec3s.erase(key);
    save();
}

void PersistentStorage::setIfNotVec3(std::string container, std::string key, Vector3 value){
    init();
    if(containers.find(container) == containers.end()){
        containers[container] = new Container();
    }
    if(containers[container]->vec3s.find(key) == containers[container]->vec3s.end()){
        containers[container]->vec3s[key] = value;
        save();
    }
}

void PersistentStorage::getOrDefaultVec3(std::string container, std::string key, Vector3 *value, Vector3 defaultValue){
    init();
    if(containers.find(container) == containers.end()){
        *value = defaultValue;
    }
    if(containers[container]->vec3s.find(key) == containers[container]->vec3s.end()){
        *value = defaultValue;
    }
    *value = containers[container]->vec3s[key];
}
