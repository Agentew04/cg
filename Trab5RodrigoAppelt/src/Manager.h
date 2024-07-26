#ifndef __MANAGER_H__
#define __MANAGER_H__

#include "Keyboard.h"
#include "Engine/Engine.h"
#include "TreeCreator.h"
#include "Engine/Components/TerrainBezier.h"
#include "Engine/Components/Character.h"

/// @brief Gerencia toda a interacao com a engine e o usuario.
/// Vai ter o codigo do jogo.
class Manager
{
public:
   /// @brief sinaliza que o jogo vai comecar
   void startloop();

   /// @brief Chamado a todo pedido de render do openGL
   void render();
   /// @brief Chamado imediatamente antes da render()
   /// @param delta O tempo desde o ultimo update
   void update(float delta);
   /// @brief Chamado quando uma tecla eh pressionada
   /// @param key A tecla pressionada
   void keyboard(Key key);
   /// @brief Chamado quando uma tecla eh solta
   /// @param key A tecla solta
   void keyboardUp(Key key);

private:
   Engine::Engine engine;
   TreeCreator treeCreator;
   std::shared_ptr<Engine::Components::TerrainBezier> terrain;
   std::shared_ptr<Engine::Components::Character> player;

   /// @brief inicializa tudo
   void createWorld();

   /// @brief Cria o terreno
   void createTerrain();
   /// @brief Cria as arvores
   void createTrees();
   /// @brief Cria o personagem
   void createCharacter();
   /// @brief Cria um ator de exemplo de Sweep
   void createSweep();
};

#endif