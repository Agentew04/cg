#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "Component.h"
#include "Camera.h"

#include "../../Keyboard.h"

namespace Engine::Components
{
    class Character: public Component {
    public:
        void Start() override;
        void Key(int key) override;
        void Update(float delta) override;

    private:
        enum class Movement {
            NONE = 0 << 0,
            LEFT = 1 << 0,
            RIGHT = 1 << 1
        };

        Movement mov;
        std::shared_ptr<Camera> linkedCamera;
    };
}; // namespace name


#endif