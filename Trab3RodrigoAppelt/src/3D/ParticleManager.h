#ifndef __PARTICLE_MANAGER_H__
#define __PARTICLE_MANAGER_H__

#include <vector>
#include <random>

#include "ObjLoader.h"
#include "../Math/Vector2.h"

class ParticleManager {
public:
    ParticleManager() = default;
    ParticleManager(int seed);
    void spawn(ObjFile *file, uint32_t count, Vector2 initialPos, Vector2 scale, std::vector<Vector3> colors, float ttl, float force, bool gravity);

    // event forwarding
    void update(float deltaTime);
    void render();
private:

    struct ParticleSpawn {
    public:
        struct Particle {
            Vector2 position;
            Vector2 velocity;
            Vector3 color;
            float life;
            float maxLife;
        };
        ObjFile *file;
        Vector2 scale;
        std::vector<Particle> particles;
        bool gravity;
    };

    const Vector2 gravity;
    std::mt19937 rng;
    std::vector<ParticleSpawn> particleSpawns;
};

#endif