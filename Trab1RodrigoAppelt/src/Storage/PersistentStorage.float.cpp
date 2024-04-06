#include "PersistentStorage.h"

void PersistentStorage::getFloat(std::string container, std::string key, float *value){
    init();
    if(containers.find(container) == containers.end()){
        return;
    }
    if(containers[container]->floats.find(key) == containers[container]->floats.end()){
        return;
    }
    *value = containers[container]->floats[key];
}

void PersistentStorage::setFloat(std::string container, std::string key, float value){
    init();
    if(containers.find(container) == containers.end()){
        containers[container] = new Container();
    }
    containers[container]->floats[key] = value;
    // save();
}

bool PersistentStorage::hasFloat(std::string container, std::string key){
    init();
    if(containers.find(container) == containers.end()){
        return false;
    }
    if(containers[container]->floats.find(key) == containers[container]->floats.end()){
        return false;
    }
    return true;
}

void PersistentStorage::removeFloat(std::string container, std::string key){
    init();
    if(containers.find(container) == containers.end()){
        return;
    }
    if(containers[container]->floats.find(key) == containers[container]->floats.end()){
        return;
    }
    containers[container]->floats.erase(key);
    // save();
}

void PersistentStorage::setIfNotFloat(std::string container, std::string key, float value){
    init();
    if(containers.find(container) == containers.end()){
        containers[container] = new Container();
    }
    if(containers[container]->floats.find(key) == containers[container]->floats.end()){
        containers[container]->floats[key] = value;
        // save();
    }
}

void PersistentStorage::getOrDefaultFloat(std::string container, std::string key, float *value, float defaultValue){
    init();
    if(containers.find(container) == containers.end()){
        *value = defaultValue;
        return;
    }
    if(containers[container]->floats.find(key) == containers[container]->floats.end()){
        *value = defaultValue;
        return;
    }
    *value = containers[container]->floats[key];
}
