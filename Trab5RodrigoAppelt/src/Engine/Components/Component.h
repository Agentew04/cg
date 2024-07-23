#ifndef __COMPONENT_H__
#define __COMPONENT_H__

#include <string>
#include <memory>

#include "../../Keyboard.h"

#ifdef ERROR
#undef ERROR
#endif

// forward declaration
namespace Engine{
    class Actor;
};

namespace Engine::Components {

    /// @brief Classe base de um componente que eh mae
    /// de todos os outros. Provem metodos virtuais para
    /// as classes derivadas implementarem.
    class Component {
    public:
        virtual ~Component() = default;

        std::weak_ptr<Actor> actor;

        /// @brief Retorna o nome do componente. Idealmente esse valor nao muda para o mesmo tipo de componente.
        /// @return O nome do componente
        virtual std::string getName() const;

        /// @brief Chamado quando a engine comeca a rodar
        virtual void Start() {};

        /// @brief Chamado a cada frame para atualizar componentes com o tempo
        /// @param delta O tempo passado desde o frame
        virtual void Update(float delta) {};

        /// @brief Chamado quando o componente eh destruido. Deve
        /// limpar todos os recursos aqui.
        virtual void Destroy() {};

        /// @brief Renderiza o componente atual.
        virtual void Render() {};

        /// @brief Chamado quando uma tecla eh pressionada
        virtual void KeyDown(Key key) {}

        /// @brief Chamado quando uma tecla eh solta
        virtual void KeyUp(Key key) {}
    protected:
    
        enum class LogLevel {
            INFO,
            WARNING,
            ERROR
        };

        /// @brief Imprime uma mensagem no console
        /// @param level Nivel de severidade
        /// @param message A mensagem a ser impressa
        void log(LogLevel level, std::string message) const;
    };
};


#endif
