#ifndef __ACTOR_H__
#define __ACTOR_H__

#include <string>
#include <vector>

#include "../Math/Vector3.h"

#include "Components/Component.h"

namespace Engine {

/// @brief Representa um ator na hierarquia da cena.
/// Tem nome, lista de componentes e lista de filhos.
class Actor{
public:
    /// @brief O nome do ator
    std::string name;
    /// @brief A posicao do ator no espaco
    Vector3 position;
    /// @brief A rotacao do ator no espaco. Angulos estao em graus.
    Vector3 rotation;
    /// @brief A escala do ator no espaco
    Vector3 scale;
    /// @brief Uma lista de componentes que este ator tem
    std::vector<Components::Component> components;
    /// @brief Uma lista de atores filhos
    std::vector<Actor> children;

    /// @brief Inicia todos os componentes deste ator
    void Start();
    /// @brief Atualiza todos os componentes deste ator e seus filhos
    void Update();
    /// @brief Destroi todos os componentes deste ator e seus filhos
    void Destroy();
    /// @brief Renderiza todos os componentes deste ator e seus filhos
    void Render();
private:
};

}; 

#endif