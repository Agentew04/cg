#include "Component.h"

#include <iostream>

#include "../Actor.h"

using namespace Engine::Components;

void Component::log(LogLevel level, std::string message) const{

    std::string id;
    if(auto act = actor.lock()){
        id = act->name + "::" + getName();
    } else {
        id = getName();
    }
    switch (level){
        case LogLevel::INFO:
            std::cout << "[INFO] [" << id << "] " << message << std::endl;
            break;
        case LogLevel::WARNING:
            std::cout << "[WARNING] [" << id << "] " << message << std::endl;
            break;
        case LogLevel::ERROR:
            std::cout << "[ERROR] [" << id << "] " << message << std::endl;
            break;
    }
}

std::string Component::getName() const {
    return "Base Component";
}
