#ifndef __TREE_CREATOR_H__
#define __TREE_CREATOR_H__

#include <map>

#include "Engine/Actor.h"
#include "Engine/Mesh.h"

class TreeCreator {
public:

    enum class TreeTop{
        Sphere,
        Tetrahedron,
        Cube
    };

    Engine::Actor createTree(TreeTop copa);
private:
    struct Tree{
        std::shared_ptr<Engine::Mesh> barkMesh;
        std::shared_ptr<Engine::Mesh> treeTopModel;
        TreeTop treeTop;
    };

    std::map<TreeTop, Tree> cachedTrees;
};

#endif