#include "Game.h"

#include "../Storage/PersistentStorage.h"


void Game::saveState(){
    //espera-se que nao e um gameover aqui
    // aqui temos que persistir:
    // simples: 
    // nivel
    // firstback
    // burstCount
    // spawned
    // hasActiveplay
    // lastbursttime
    // balllaunchposition
    // balllaunchdirection
    // nextlaunchposition
    // vetores:
    // blocos(posicao e vida)
    // bolas(posicao e velocidade)
    // powerups de bola(posicao na grid)
    // powerups de laser(posicao na grid, orientacao, se estah dirty)
    
    //simples:
    PersistentStorage::set<int>("game", "level", level);
    PersistentStorage::set<bool>("game", "firstBack", firstBack);
    PersistentStorage::set<uint32_t>("game", "burstCount", burstCount);
    PersistentStorage::set<uint32_t>("game", "spawned", spawned);
    PersistentStorage::set<bool>("game", "hasActivePlay", hasActivePlay);
    // PersistentStorage::set<float>("game", "lastBurstTime", lastBurstTime);
    PersistentStorage::set<Vector2>("game", "ballLaunchPosition", ballLaunchPosition);
    PersistentStorage::set<Vector2>("game", "ballLaunchDirection", ballLaunchDirection);
    PersistentStorage::set<Vector2>("game", "nextLaunchPosition", nextLaunchPosition);

    //vetores:
    // blocos
    //aqui vamos macaquear pq a serializacao de vetor n ta funcionando
    //azar
    // acumular linhas em um unico vetor
    std::vector<Block> blocos;
    for(auto &line: blockLines){
        for(auto &block: line){
            blocos.push_back(block);
        }
    }
    PersistentStorage::set<int>("game", "blockCount", blocos.size());
    int i=0;
    for(auto& block: blocos){
        PersistentStorage::set<Vector2>(
            "game", 
            "block"+std::to_string(i)+"pos",
            block.position);
        PersistentStorage::set<Vector3>(
            "game", 
            "block"+std::to_string(i)+"color",
            block.color);
        PersistentStorage::set<int>(
            "game", 
            "block"+std::to_string(i)+"life",
            block.life);
        i++;
    }

    // bolas, fazer a msm coisa pq sim
    PersistentStorage::set<int>("game", "ballsCount", balls.size());
    i=0;
    for(auto& ball: balls){
        PersistentStorage::set<Vector2>(
            "game", 
            "ball"+std::to_string(i)+"pos",
            ball.position);
        PersistentStorage::set<Vector2>(
            "game", 
            "ball"+std::to_string(i)+"vel",
            ball.velocity);
        i++;
    }

    // powerups de bola
    PersistentStorage::set<int>("game", "powerupBallsCount", ballPowerups.size());
    i=0;
    for(auto& powerup: ballPowerups){
        PersistentStorage::set<Vector2>(
            "game", 
            "powerupBall"+std::to_string(i)+"pos",
            powerup.position);
        i++;
    }

    // powerups de laser
    PersistentStorage::set<int>("game", "powerupLasersCount", laserPowerups.size());
    i=0;
    for(auto& powerup: laserPowerups){
        PersistentStorage::set<Vector2>(
            "game", 
            "powerupLaser"+std::to_string(i)+"pos",
            powerup.position);
        PersistentStorage::set<int>(
            "game", 
            "powerupLaser"+std::to_string(i)+"dir",
            (int)powerup.direction);
        PersistentStorage::set<bool>(
            "game", 
            "powerupLaser"+std::to_string(i)+"dirty",
            powerup.dirty);
        i++;
    }
}

void Game::loadState(){
    reset();
    // aqui temos que carregar de volta pra memoria:
    // simples: 
    // nivel
    // firstback
    // burstCount
    // spawned
    // hasActiveplay
    // lastbursttime
    // balllaunchposition
    // balllaunchdirection
    // nextlaunchposition
    // vetores:
    // blocos(posicao e vida)
    // bolas(posicao e velocidade)
    // powerups de bola(posicao na grid)
    // powerups de laser(posicao na grid, orientacao, se estah dirty)

    //simples:

    level = PersistentStorage::get<int>("game", "level", 1);
    firstBack = PersistentStorage::get<bool>("game", "firstBack", false);
    burstCount = PersistentStorage::get<uint32_t>("game", "burstCount", 1);
    spawned = PersistentStorage::get<uint32_t>("game", "spawned", 0);
    hasActivePlay = PersistentStorage::get<bool>("game", "hasActivePlay", false);
    lastBurstTime = 0;
    // lastBurstTime = PersistentStorage::get<float>("game", "lastBurstTime", 0.0f);
    ballLaunchPosition = PersistentStorage::get<Vector2>("game", "ballLaunchPosition", Vector2::zero());
    ballLaunchDirection = PersistentStorage::get<Vector2>("game", "ballLaunchDirection", Vector2::zero());
    nextLaunchPosition = PersistentStorage::get<Vector2>("game", "nextLaunchPosition", Vector2::zero());

    //vetores:
    // blocos
    // desfazer a macaqueada
    int blockCount = PersistentStorage::get<int>("game", "blockCount", 0);
    std::vector<Block> blocos;
    for(int i=0;i<blockCount;i++){
        Vector2 pos = PersistentStorage::get<Vector2>(
            "game", 
            "block"+std::to_string(i)+"pos",
            Vector2::zero());
        Vector3 color = PersistentStorage::get<Vector3>(
            "game", 
            "block"+std::to_string(i)+"color",
            Vector3::zero());
        int life = PersistentStorage::get<int>(
            "game", 
            "block"+std::to_string(i)+"life",
            0);
        Block b(pos, color, life);
        blocos.push_back(b);
    }
    // agregar blocos em linhas
    blockLines.clear();
    //fodasse eu vou jogar um bloco em cada linha azar
    for(auto &block: blocos){
        std::vector<Block> linha;
        linha.push_back(block);
        blockLines.push_back(linha);
    }

    for(auto &line: blockLines){
        for(auto &block: line){
            block.collider.id = rng();
        }
    }

    // bolas, mesma macaquice
    int ballsCount = PersistentStorage::get<int>("game", "ballsCount", 0);
    for(int i=0;i<ballsCount;i++){
        Ball b;
        b.position = PersistentStorage::get<Vector2>(
            "game", 
            "ball"+std::to_string(i)+"pos",
            Vector2::zero());
        b.velocity = PersistentStorage::get<Vector2>(
            "game", 
            "ball"+std::to_string(i)+"vel",
            Vector2::zero());
        balls.push_back(b);
    }

    for(auto &ball: balls){
        ball.collider.id = rng();
    }

    // powerups de bola
    int powerupBallsCount = PersistentStorage::get<int>("game", "powerupBallsCount", 0);
    for(int i=0;i<powerupBallsCount;i++){
        ExtraBall p(PersistentStorage::get<Vector2>(
            "game", 
            "powerupBall"+std::to_string(i)+"pos",
            Vector2::zero()));
        ballPowerups.push_back(p);
    }

    // powerups de laser
    int powerupLasersCount = PersistentStorage::get<int>("game", "powerupLasersCount", 0);
    for(int i=0;i<powerupLasersCount;i++){
        Vector2 pos = PersistentStorage::get<Vector2>(
            "game", 
            "powerupLaser"+std::to_string(i)+"pos",
            Vector2::zero());
        Laser::Direction dir = (Laser::Direction)PersistentStorage::get<int>(
            "game", 
            "powerupLaser"+std::to_string(i)+"dir",
            0);
        bool dirty = PersistentStorage::get<bool>(
            "game", 
            "powerupLaser"+std::to_string(i)+"dirty",
            false);
        Laser p(pos, dir);
        p.dirty = dirty;
        laserPowerups.push_back(p);
    }
}