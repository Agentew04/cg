#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <string>
#include <memory>

#ifdef ERROR
#undef ERROR
#endif


namespace Engine{
    class Actor;
};

namespace Engine::Components {
    class Component {
    public:
        std::shared_ptr<Actor> actor;
        
        virtual std::string getName() const;
        virtual void Start() = 0;
        virtual void Update(float delta) = 0;
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
