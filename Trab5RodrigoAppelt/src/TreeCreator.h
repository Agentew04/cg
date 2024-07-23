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
        Cube,
        Cone,
        COUNT
    };

    std::shared_ptr<Engine::Actor> createTree(TreeTop top);
private:

    std::shared_ptr<Engine::Mesh> barkMesh;

    // struct soh pra caso precise add mais modelos
    struct Tree{
        std::shared_ptr<Engine::Mesh> treeTopModel;
    };

    std::map<TreeTop, Tree> cachedTrees;

    GLuint barkTextureId = 0;
    GLuint treeTopTextureId = 0;
    std::shared_ptr<Engine::Mesh> createBark();
    std::shared_ptr<Engine::Mesh> createTreeTop(TreeTop top);
};

#endif