#ifndef __ROTATING_H__
#define __ROTATING_H__

#include "Component.h"
#include "../Actor.h"
#include <GL/glut.h>

namespace Engine::Components {
    /// @brief Componente que faz um objeto rotacionar.
    class Rotating : public Component {
    public:
        std::string getName() const override { return "Rotating"; }
        
        /// @brief Atualiza o componente.
        void Update(float delta) override {
            if(auto act = actor.lock()){
                log(LogLevel::INFO, "Rotating " + act->name + " " + std::to_string(angle));
                angle += speed * delta;
                act->rotation.y = angle;
            }
        }
                
        /// @brief Velocidade de rotacao.
        float speed = 5;
    private:
        /// @brief Angulo de rotacao.
        float angle = 0;
    };
};

#endif