#include "Component.h"

#include <iostream>

using namespace Engine::Components;

void Component::log(LogLevel level, std::string message){
    switch (level){
        case LogLevel::INFO:
            std::cout << "[INFO] [" << actor->name << "::" << name << "] " << message << std::endl;
            break;
        case LogLevel::WARNING:
            std::cout << "[WARNING] [" << actor->name << "::" << name << "] " << message << std::endl;
            break;
        case LogLevel::ERROR:
            std::cout << "[ERROR] [" << actor->name << "::" << name << "] " << message << std::endl;
            break;
    }
}