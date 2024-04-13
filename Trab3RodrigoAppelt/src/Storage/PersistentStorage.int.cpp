#include "PersistentStorage.h"

void PersistentStorage::getInt(std::string container, std::string key, int *value){
    init();
    if(containers.find(container) == containers.end()){
        return;
    }
    if(containers[container]->ints.find(key) == containers[container]->ints.end()){
        return;
    }
    *value = containers[container]->ints[key];
}

void PersistentStorage::setInt(std::string container, std::string key, int value){
    init();
    if(containers.find(container) == containers.end()){
        containers[container] = new Container();
    }
    containers[container]->ints[key] = value;
    // save();
}

bool PersistentStorage::hasInt(std::string container, std::string key){
    init();
    if(containers.find(container) == containers.end()){
        return false;
    }
    if(containers[container]->ints.find(key) == containers[container]->ints.end()){
        return false;
    }
    return true;
}

void PersistentStorage::removeInt(std::string container, std::string key){
    init();
    if(containers.find(container) == containers.end()){
        return;
    }
    if(containers[container]->ints.find(key) == containers[container]->ints.end()){
        return;
    }
    containers[container]->ints.erase(key);
    // save();
}

void PersistentStorage::setIfNotInt(std::string container, std::string key, int value){
    init();
    if(containers.find(container) == containers.end()){
        containers[container] = new Container();
    }
    if(containers[container]->ints.find(key) == containers[container]->ints.end()){
        containers[container]->ints[key] = value;
        // save();
    }
}

void PersistentStorage::getOrDefaultInt(std::string container, std::string key, int *value, int defaultValue){
    init();
    if(containers.find(container) == containers.end()){
        *value = defaultValue;
        return;
    }
    if(containers[container]->ints.find(key) == containers[container]->ints.end()){
        *value = defaultValue;
        containers[container]->ints[key] = defaultValue;
        return;
    }
    *value = containers[container]->ints[key];
}