#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <vector>
#include <list>
#include <memory>
#include <optional>

#include "Actor.h"
#include "Mesh.h"

#include "../Math/Vector3.h"

namespace Engine
{

   /// @brief Classe principal da engine. Gerencia todos os atores
   /// e event forwarding.
   class Engine
   {
   public:
      /// @brief Unica instancia usada da engine.
      static Engine *instance;
      /// @brief Uma lista com os atores raizes da cena.
      std::vector<std::shared_ptr<Actor>> hierarchy;

      /// @brief Retorna o primeiro ator com o nome especificado.
      /// @param name O nome a ser procurado
      /// @return O ator com o nome especificado, ou std::nullopt se nao encontrar
      std::optional<std::shared_ptr<Actor>> FindActorByName(const std::string &name);

      /// @brief Retorna o primeiro ator que contem o componente dado.
      /// @tparam T O tipo de componente. Deve ser derivado de Engine::Components::Component
      /// @return O primeiro ator que contem o componente dado, ou std::nullopt se nao encontrar
      template <typename T>
      std::optional<std::shared_ptr<Actor>> FindActorWithComponent()
      {
         static_assert(std::is_base_of<Components::Component, T>::value);
         for (auto actor : hierarchy)
         {
            auto ret = _FindActorWithComponent<T>(actor);
            if (ret.has_value())
            {
               return ret;
            }
         }
         return std::nullopt;
      }

      /// @brief Retorna todos os componentes da cena que tem o tipo passado.
      /// @tparam T O tipo de componente desejado
      /// @return Uma lista com todos os componentes desse tipo achados.
      template <typename T>
      std::vector<std::shared_ptr<T>> GetAllComponentsOfType() const
      {
         static_assert(std::is_base_of<Components::Component, T>::value);
         std::vector<std::shared_ptr<T>> components;
         for (auto actor : hierarchy)
         {
            auto comps = _GetAllComponentsOfType<T>(actor);
            components.insert(components.end(), comps.begin(), comps.end());
         }
         return components;
      }

      /// @brief Especie de cache com todas as meshes carregadas e em uso
      std::list<std::shared_ptr<Mesh>> loadedMeshes;

      /// @brief Inicializa todos os atores e prepara a engine
      void Start();
      /// @brief Roda um tick de update em todos os componentes de todos os atores
      void Update(float delta);
      /// @brief Destroi tudo.
      void Destroy();
      /// @brief Renderiza um frame.
      void Render();

      /// @brief Repassa evento de tecla pressionada para todos os componentes
      void KeyDown(int key);
      /// @brief Repassa evento de tecla solta para todos os componentes
      void KeyUp(int key);

   private:
      // private helper
      std::optional<std::shared_ptr<Actor>> _FindActorByName(std::shared_ptr<Actor> root, const std::string &name);

      template <typename T>
      std::optional<std::shared_ptr<Actor>> _FindActorWithComponent(std::shared_ptr<Actor> root)
      {
         static_assert(std::is_base_of<Components::Component, T>::value);
         if (root->HasComponent<T>())
         {
            return std::optional(root);
         }
         for (auto child : root->children)
         {
            auto ret = FindActorWithComponent<T>(child);
            if (ret.has_value())
            {
               return ret;
            }
         }
         return std::nullopt;
      }

      template <typename T>
      std::vector<std::shared_ptr<T>> _GetAllComponentsOfType(std::shared_ptr<Actor> root) const
      {
         static_assert(std::is_base_of<Components::Component, T>::value);
         std::vector<std::shared_ptr<T>> components;
         for (auto component : root->components)
         {
            if (dynamic_cast<T *>(component.get()))
            {
               components.push_back(std::dynamic_pointer_cast<T>(component));
            }
         }
         for (const auto child : root->children)
         {
            auto comps = _GetAllComponentsOfType<T>(child);
            components.insert(components.end(), comps.begin(), comps.end());
         }
         return components;
      }

      Vector3 cameraPos = Vector3(0, 0, 0);
      Vector3 cameraLookAt = Vector3(0, 0, 1);
   };
};

#endif
