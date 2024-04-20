#include "ParticleManager.h"

#include <cmath>

#include "../gl_canvas2d.h"

ParticleManager::ParticleManager(int seed) : 
    gravity(0, +9.81*50),
    rng(seed) {}

void ParticleManager::spawn(ObjFile *file, uint32_t count, Vector2 initialPos, Vector2 scale, std::vector<Vector3> colors, float ttl, float force, bool gravity){
    ParticleSpawn spn;
    spn.file = file;
    spn.gravity = gravity;
    spn.scale = scale;

    std::uniform_int_distribution<uint32_t> colorsDistrib(0, colors.size()-1);
    std::uniform_real_distribution<float> ttlRng(0, ttl*0.3);    

    for(int i=0; i<count; i++){
        ParticleSpawn::Particle particle;
        particle.position = initialPos;
        
        float step = (2*3.1415)/count;
        float x = force * std::cos(step * i);
        float y = force * std::sin(step * i);

        particle.velocity = Vector2(x,y);
        particle.life = 0;
        particle.maxLife = ttl + ttlRng(rng);
        particle.color = colors.at(colorsDistrib(rng));

        spn.particles.push_back(particle);
    }

    particleSpawns.push_back(spn);
}

void ParticleManager::update(float delta){
    for(auto &spawn:particleSpawns){
        for(auto it=spawn.particles.begin(); it < spawn.particles.end(); it++){
            ParticleSpawn::Particle& particle = *it;
            if(particle.life > particle.maxLife ){
                spawn.particles.erase(it);
                continue;
            }

            particle.position += particle.velocity * delta;

            if(spawn.gravity){
                particle.velocity += gravity * delta;
            }
            particle.life += delta;
        }
    }
    
    for(auto it=particleSpawns.begin(); it<particleSpawns.end(); it++){
        if(it->particles.size() == 0){
            particleSpawns.erase(it);
        }
    }
}

void ParticleManager::render(){
    CV::translate(0,0);
    for(auto &spawn:particleSpawns){
        for(auto &particle:spawn.particles){
            CV::color(particle.color);
            CV::obj(spawn.file, particle.position, spawn.scale);
        }
    }
}