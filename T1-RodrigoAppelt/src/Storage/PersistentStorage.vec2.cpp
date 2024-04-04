#include "PersistentStorage.h"

void PersistentStorage::getVec2(std::string container, std::string key, Vector2 *value){
    init();
    if(containers.find(container) == containers.end()){
        return;
    }
    if(containers[container]->vec2s.find(key) == containers[container]->vec2s.end()){
        return;
    }
    *value = containers[container]->vec2s[key];
}

void PersistentStorage::setVec2(std::string container, std::string key, Vector2 value){
    init();
    if(containers.find(container) == containers.end()){
        containers[container] = new Container();
    }
    containers[container]->vec2s[key] = value;
    save();
}

bool PersistentStorage::hasVec2(std::string container, std::string key){
    init();
    if(containers.find(container) == containers.end()){
        return false;
    }
    if(containers[container]->vec2s.find(key) == containers[container]->vec2s.end()){
        return false;
    }
    return true;
}

void PersistentStorage::removeVec2(std::string container, std::string key){
    init();
    if(containers.find(container) == containers.end()){
        return;
    }
    if(containers[container]->vec2s.find(key) == containers[container]->vec2s.end()){
        return;
    }
    containers[container]->vec2s.erase(key);
    save();
}

void PersistentStorage::setIfNotVec2(std::string container, std::string key, Vector2 value){
    init();
    if(containers.find(container) == containers.end()){
        containers[container] = new Container();
    }
    if(containers[container]->vec2s.find(key) == containers[container]->vec2s.end()){
        containers[container]->vec2s[key] = value;
        save();
    }
}

void PersistentStorage::getOrDefaultVec2(std::string container, std::string key, Vector2 *value, Vector2 defaultValue){
    init();
    if(containers.find(container) == containers.end()){
        *value = defaultValue;
    }
    if(containers[container]->vec2s.find(key) == containers[container]->vec2s.end()){
        *value = defaultValue;
    }
    *value = containers[container]->vec2s[key];
}
