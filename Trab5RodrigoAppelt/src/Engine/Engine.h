#ifndef __ENGINE_H__
#define __ENGINE_H__

#include <vector>
#include <list>
#include <memory>

#include "Actor.h"
#include "Mesh.h"

namespace Engine{

    class Engine {
        public:
            std::vector<Actor> hierarchy;
            template <typename T>
            void FindActorByName(std::string name, std::vector<T> &actors){
                for (auto actor : hierarchy){
                    if (actor.name == name){
                        actors.push_back(actor);
                    }
                    for (auto child : actor.children){
                        FindActorByName(name, actors);
                    }
                }
            }

            template <typename T>
            void FindActorWithComponent(std::string type, std::vector<T> &actors){
                for (auto actor : hierarchy){
                    for (auto component : actor.components){
                        if (component.name == type){
                            actors.push_back(actor);
                        }
                    }
                    for (auto child : actor.children){
                        FindActorByType(type, actors);
                    }
                }
            }

            std::list<std::shared_ptr<Mesh>> loadedMeshes;

            void Update();
            void Destroy();
            void Render();
    };
};

#endif