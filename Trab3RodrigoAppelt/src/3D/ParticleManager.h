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
    /// @brief Cria um efeito de particulas
    /// @param file O arquivo 3d usado para renderizar as particulas. E interessante ser um arquivo otimizado
    /// @param count Quantas particulas a serem renderizadas
    /// @param initialPos A posicao que elas vao ser criadas
    /// @param scale A escala do arquivo .obj a ser utilizada
    /// @param colors As cores disponiveis. Cada particula vai ter uma cor aleatoria daqui
    /// @param ttl O tempo minimo de vida de cada particula. O tempo final eh isso + até 10% do ttl
    /// @param force A força que as particulas sao jogadas para fora do centro
    /// @param gravity Se elas sao afetaddas pela gravidade ou nao
    void spawn(ObjFile *file, uint32_t count, Vector2 initialPos, Vector2 scale, std::vector<Vector3> colors, float ttl, float force, bool gravity);

    /// @brief Deleta todas as particulas que estao aparecento imediatamente
    void reset();

    /// @brief Verifica se ha particulas existentes aparecendo
    /// @return 'true' se tem ou 'false' senao.
    bool hasParticles();

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