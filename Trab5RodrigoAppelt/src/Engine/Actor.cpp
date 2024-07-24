#include "Actor.h"

#include <GL/glut.h>
#include "Components/Camera.h"

using namespace Engine;

void Actor::Start()
{
   for (auto &component : components)
   {
      component->Start();
   }
   for (auto &child : children)
   {
      child->Start();
   }
}

void Actor::Update(float delta)
{
   for (auto &component : components)
   {
      component->Update(delta);
   }
   for (auto &child : children)
   {
      child->Update(delta);
   }
}

void Actor::Destroy()
{
   for (auto &child : children)
   {
      child->Destroy();
   }
   for (auto &component : components)
   {
      component->Destroy();
   }

   components.clear();
   children.clear();
}

void Actor::Render()
{
   glPushMatrix();

   glTranslatef(position.x, position.y, position.z);
   glScalef(scale.x, scale.y, scale.z);
   glRotatef(rotation.x, 1, 0, 0);
   glRotatef(rotation.y, 0, 1, 0);
   glRotatef(rotation.z, 0, 0, 1);

   for (auto &component : components)
   {
      component->Render();
   }
   for (auto &child : children)
   {
      child->Render();
   }

   glPopMatrix();
}

void Actor::KeyDown(Key key)
{
   for (auto &component : components)
   {
      component->KeyDown(key);
   }
   for (auto &child : children)
   {
      child->KeyDown(key);
   }
}

void Actor::KeyUp(Key key)
{
   for (auto &component : components)
   {
      component->KeyUp(key);
   }
   for (auto &child : children)
   {
      child->KeyUp(key);
   }
}

Vector3 Actor::getForward() const
{
   auto fwd = Vector3(0, 0, 1);
   if (auto prnt = parent.lock())
   {
      fwd = prnt->getForward();
   }
   fwd = fwd.rotate(rotation.x * (3.1415 / 180.0), Vector3(1, 0, 0));
   fwd = fwd.rotate(rotation.y * (3.1415 / 180.0), Vector3(0, 1, 0));
   fwd = fwd.rotate(rotation.z * (3.1415 / 180.0), Vector3(0, 0, 1));
   return fwd;
}

Vector3 Actor::getRight() const
{
   auto right = Vector3(1, 0, 0);
   if (auto prnt = parent.lock())
   {
      right = prnt->getRight();
   }
   right = right.rotate(rotation.x * (3.1415 / 180.0), Vector3(1, 0, 0));
   right = right.rotate(rotation.y * (3.1415 / 180.0), Vector3(0, 1, 0));
   right = right.rotate(rotation.z * (3.1415 / 180.0), Vector3(0, 0, 1));
   return right;
}

Vector3 Actor::getUp() const
{
   auto up = Vector3(0, 1, 0);
   if (auto prnt = parent.lock())
   {
      up = prnt->getUp();
   }
   up = up.rotate(rotation.x * (3.1415 / 180.0), Vector3(1, 0, 0));
   up = up.rotate(rotation.y * (3.1415 / 180.0), Vector3(0, 1, 0));
   up = up.rotate(rotation.z * (3.1415 / 180.0), Vector3(0, 0, 1));
   return up;
}

Vector3 Actor::getGlobalPosition() const
{
   auto basepos = Vector3(0, 0, 0);
   if (auto prnt = parent.lock())
   {
      basepos = prnt->getGlobalPosition();
   }
   return basepos + position;
}

void Actor::addChild(std::shared_ptr<Actor> child)
{
   child->parent = shared_from_this();
   children.push_back(child);
}
