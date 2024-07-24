#include "TreeCreator.h"

#include <memory>
#include <map>

#include "Engine/Components/MeshRenderer.h"
#include "Engine/MeshImporter.h"
#include "Engine/Mesh.h"

std::shared_ptr<Engine::Actor> TreeCreator::createTree(TreeTop top)
{
   auto barkActor = std::make_shared<Engine::Actor>("TreeBark");
   auto topActor = std::make_shared<Engine::Actor>("TreeTop");
   topActor->position = Vector3(0, 8, 0);
   topActor->scale = Vector3(3, 3, 3);

   auto barkMr = std::make_shared<Engine::Components::MeshRenderer>();
   auto topMr = std::make_shared<Engine::Components::MeshRenderer>();

   if (barkTextureId == 0)
   {
      barkTextureId = Engine::Mesh::loadTexture(".\\Trab5RodrigoAppelt\\assets\\images\\bark.png");
   }
   if (treeTopTextureId == 0)
   {
      treeTopTextureId = Engine::Mesh::loadTexture(".\\Trab5RodrigoAppelt\\assets\\images\\leaves.png");
   }

   barkMr->useTexture(barkTextureId);
   topMr->useTexture(treeTopTextureId);

   if (!barkMesh)
   {
      barkMesh = createBark();
   }

   barkMr->mesh = barkMesh;

   // gerar mesh ou pegar uma da cache
   auto it = cachedTrees.find(top);
   if (it != cachedTrees.end())
   {
      auto tree = it->second;
      topMr->mesh = tree.treeTopModel;
   }
   else
   {
      // create meshes and add to cache
      Tree t;
      t.treeTopModel = createTreeTop(top);
      cachedTrees[top] = t;
      topMr->mesh = t.treeTopModel;
   }

   // add mesh renderers for each
   barkActor->addComponent(std::move(barkMr));
   topActor->addComponent(std::move(topMr));

   barkActor->addChild(std::move(topActor));
   return barkActor;
}

std::shared_ptr<Engine::Mesh> TreeCreator::createBark()
{
   auto bark = Engine::MeshImporter::loadMesh(".\\Trab5RodrigoAppelt\\assets\\models\\treebark.3d");
   // forcar material marrom
   for (auto &m : bark->materials)
   {
      m.setDiffuse(122 / 255.0, 69 / 255.0, 0.0, 1.0);
      m.setAmbient(63 / 255.0, 36 / 255.0, 0.0, 1.0);
      m.setSpecular(1, 1, 1, 1);
      m.shininess = 100;
   }
   return bark;
}

std::shared_ptr<Engine::Mesh> TreeCreator::createTreeTop(TreeTop top)
{
   std::shared_ptr<Engine::Mesh> mesh;

   switch (top)
   {
   case TreeTop::Sphere:
      mesh = Engine::MeshImporter::loadMesh(".\\Trab5RodrigoAppelt\\assets\\models\\treetop_sphere.3d");
      break;
   case TreeTop::Tetrahedron:
   {
      // runtime generation
      auto tetra = Engine::Mesh::tetrahedron(4);
      mesh = std::make_shared<Engine::Mesh>(tetra);
   }
   break;
   case TreeTop::Cube:
      mesh = Engine::MeshImporter::loadMesh(".\\Trab5RodrigoAppelt\\assets\\models\\treetop_cube.3d");
      break;
   case TreeTop::Cone:
      mesh = Engine::MeshImporter::loadMesh(".\\Trab5RodrigoAppelt\\assets\\models\\treetop_cone.3d");
      break;
   default:
      std::cout << "Unknown treetop:" << (int)top << std::endl;
      mesh = nullptr;
      break;
   }
   Engine::Material treeTopMat = Engine::Material();
   treeTopMat.setDiffuse(0.0, 0.8, 0.0, 1.0);
   treeTopMat.setAmbient(0.1, 0.4, 0.1, 1.0);
   treeTopMat.shininess = 10;
   treeTopMat.setSpecular(1, 1, 1, 1);
   if (mesh && mesh->materials.size() == 0)
   {
      mesh->materials.push_back(treeTopMat);
   }
   else if (mesh)
   {
      mesh->materials[0] = treeTopMat;
   }

   return mesh;
}
