#ifndef __CHARACTER_H__
#define __CHARACTER_H__

#include "Component.h"
#include "Camera.h"

#include "../../Keyboard.h"

namespace Engine::Components
{
    class Character: public Component {
    public:
        void KeyDown(int key) override;
        void KeyUp(int key) override;
        void Update(float delta) override;

    private:
        float forwardSpeed = 10.0f;
        enum class Movement {
            NONE = 0 << 0,
            LEFT = 1 << 0,
            RIGHT = 1 << 1,
            UP = 1 << 2,
            DOWN = 1 << 3,
            FRONT = 1 << 4
        };

        Movement mov;
    };
}; // namespace name


#endif