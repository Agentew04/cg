#ifndef __TERRAIN_BEZIER_H__
#define __TERRAIN_BEZIER_H__

#include "Terrain.h"

namespace Engine::Components {
    class TerrainBezier : public Terrain {
    public:
        std::string name = "TerrainBezier";
        void Start() override;
        void Update() override;
        void Destroy() override;
        void Render() override;
    };
};

#endif