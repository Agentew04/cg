#ifndef __ACTOR_H__
#define __ACTOR_H__

#include <string>
#include <vector>

#include "../Math/Vector3.h"

#include "Components/Component.h"

namespace Engine {

/// @brief Representa um ator na hierarquia da cena.
/// Tem nome, lista de componentes e lista de filhos.
class Actor : public std::enable_shared_from_this<Actor>{
public:
    Actor(std::string name = "") : 
        name(name), 
        position(Vector3(0,0,0)),
        rotation(Vector3(0,0,0)),
        scale(Vector3(1,1,1)),
        components({}),
        children({}) {}

    /// @brief O nome do ator
    std::string name;
    /// @brief A posicao do ator no espaco
    Vector3 position;
    /// @brief A rotacao do ator no espaco. Angulos estao em graus.
    Vector3 rotation;
    /// @brief A escala do ator no espaco
    Vector3 scale;
    /// @brief Uma lista de componentes que este ator tem
    std::vector<std::shared_ptr<Components::Component>> components;
    /// @brief Uma lista de atores filhos
    std::vector<std::shared_ptr<Actor>> children;

    /// @brief Ponteiro para o pai.
    std::weak_ptr<Actor> parent;

    /// @brief Inicia todos os componentes deste ator
    void Start();
    /// @brief Atualiza todos os componentes deste ator e seus filhos
    void Update(float delta);
    /// @brief Destroi todos os componentes deste ator e seus filhos
    void Destroy();
    /// @brief Renderiza todos os componentes deste ator e seus filhos
    void Render();
    /// @brief Notifica componentes que uma tecla foi pressionada
    void KeyDown(Key key);
    /// @brief Notifica componentes que uma tecla foi solta
    void KeyUp(Key key);

    template <typename T>
    bool HasComponent(){
        static_assert(std::is_base_of<Components::Component,T>::value);
        for (auto& component : components){
            if (dynamic_cast<T*>(component.get())){
                return true;
            }
        }
        return false;
    }

    template <typename T>
    const T& GetComponent(){
        static_assert(std::is_base_of<Components::Component,T>::value);
        for (auto& component : components){
            if (dynamic_cast<T*>(component.get())){
                return *dynamic_cast<T*>(component.get());
            }
        }
        throw std::runtime_error("Component not found");
    }

    /// @brief Retorna a direcao para a frente deste ator.
    /// Eh cumulativo com o pai.
    Vector3 getForward() const;

    Vector3 getRight() const;

    Vector3 getUp() const;

    /// @brief Retorna a posicao global do objeto. leva em consideracao o pai.
    /// @return 
    Vector3 getGlobalPosition() const;

    /// @brief Adiciona um filho a este ator
    /// @param child O filho a ser adicionado
    void addChild(std::shared_ptr<Actor> child);

    template <typename T>
    void addComponent(std::shared_ptr<T> component){ 
        static_assert(std::is_base_of<Components::Component,T>::value);
        component->actor = shared_from_this();
        components.push_back(component);
    }
private:
};

};

#endif
