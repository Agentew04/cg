#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <string>
#include <memory>

#include "../../Keyboard.h"

#ifdef ERROR
#undef ERROR
#endif


namespace Engine{
    class Actor;
};

namespace Engine::Components {
    class Component {
    public:
        virtual ~Component() = default;

        std::weak_ptr<Actor> actor;

        virtual std::string getName() const;
        virtual void Start() {};
        virtual void Update(float delta) {};
        virtual void Destroy() {};
        virtual void Render() {};
        virtual void KeyDown(Key key) {}
        virtual void KeyUp(Key key) {}
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
