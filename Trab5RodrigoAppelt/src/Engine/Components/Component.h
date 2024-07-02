#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <string>

#include "../Actor.h"

namespace Engine::Components {
    class Component {
    public:
        std::shared_ptr<Actor> actor;
        std::string name = "Base Component";
        virtual void Start() = 0;
        virtual void Update() = 0;
        virtual void Destroy() = 0;
        virtual void Render() = 0;
    protected:
        enum class LogLevel {
            INFO,
            WARNING,
            ERROR
        };

        void log(LogLevel level, std::string message);
    };
};


#endif