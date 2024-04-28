#include "Game.h"

#include <string>
#include <functional>
#include <random>

#include "../gl_canvas2d.h"
#include "../Storage/PersistentStorage.h"
#include "../Misc/TaskManager.h"
#include "../Misc/SoundPlayer.h"

Game::Game(int *scrW, int *scrH) : rng(time(nullptr)),
                                   particleManager(rng()),
                                   screenWidth(scrW), screenHeight(scrH),
                                   mousePos(Vector2::zero()),
                                   hasActivePlay(false),
                                   burstCount(2),
                                   spawned(0),
                                   lastBurstTime(0.0f),
                                   level(1),
                                   firstBack(false),
                                   gameOver(false),
                                   ballLaunchPosition((*scrW) / 2, 8*(*scrH/9)),
                                   ballLaunchDirection(Vector2::zero()),
                                   nextLaunchPosition(Vector2::zero()),
                                   ballSpeed(200.0f),
                                   ballSpeedMultiplier(1.0f)
{
    // std::random_device r; <- versao 8.1.0 do g++ do codeblocks n suporta
    //                          passar a seed antiga direto pro mersene
    blockColors = {
        Vector3::fromHex(0xffbe0b),
        Vector3::fromHex(0xe63946),
        Vector3::fromHex(0x8338ec),
        Vector3::fromHex(0x3a86ff),
        Vector3::fromHex(0x8d99ae),
        Vector3::fromHex(0x003049),
        Vector3::fromHex(0x84a98c),
        Vector3::fromHex(0x386641),
        Vector3::fromHex(0x17c3b2),
        Vector3::fromHex(0x98c1d9),
        Vector3::fromHex(0x80ed99)
    }; //suficiente p/ loopar e nao repetir cores na tela!
    blockLines.push_back(createNewLine(level));
    pushLines();

}

Game::~Game()
{
}

void Game::reset()
{
    hasActivePlay = false;
    particleManager.reset();
    burstCount = 1;
    spawned = 0;
    level = 1;
    firstBack = false;
    lastBurstTime = 0.0f;
    ballLaunchPosition = Vector2((*screenWidth) / 2, 8 * (*screenHeight) / 9);
    ballLaunchDirection = Vector2::zero();
    firstBack = false;
    gameOver = false;
    ballSpeedMultiplier = 1.0f;
    balls.clear();
    powerups.clear();
    blockLines.clear();
    blockLines.push_back(createNewLine(1));
    pushLines();
}

void Game::update(float delta)
{
    particleManager.update(delta);
    if (hasActivePlay)
    {
        // dispara nova bola na rajada se possivel
        if(burstCount > spawned && CV::time() > lastBurstTime + burstInterval){
            Ball b;
            b.position = ballLaunchPosition;
            b.velocity = ballLaunchDirection*ballSpeed;
            b.collider = Circle2D(b.position, b.radius);
            b.collider.id = rng();
            balls.push_back(b);
            spawned++;
            lastBurstTime = CV::time();
        }

        // se por algum acaso uma bolinha perdeu o id de colisor, regenerar
        for(auto &ball: balls){
            if(ball.collider.id == 0){
                ball.collider.id = rng();
            }
        }

        //move bolinhas
        for(auto &ball: balls){
            ball.position += ball.velocity * delta * ballSpeedMultiplier;
        }

        // atualiza os colisores
        {
            Vector2 gameAreaStart(5 * (*screenWidth) / 14, 1 * (*screenHeight) / 9);
            Vector2 gameAreaSize(4 * (*screenWidth) / 14, 7 * (*screenHeight) / 9);
            float blockSize = std::min(gameAreaSize.x / blockAreaSize.x, gameAreaSize.y / blockAreaSize.y);

            for(auto &line: blockLines){
                for(auto &block: line){
                    block.collider.position = gameAreaStart + block.position*blockSize;
                    block.collider.size = Vector2(blockSize, blockSize);
                }
            }
            for(auto &ball: balls){
                ball.collider.position = ball.position;
                ball.collider.radius = ball.radius;
            }
            updateGameAreaBounds();
        }


        //verifica colisoes
        for(auto &ball: balls){
            // verifica colisoes com os blocos
            for(auto &line: blockLines){
                // iterator for para ir deletando conforme quebram
                for(auto &block:line){
                    processCollisions(ball, block);
                }
            }
            // remove blocos com 0 de vida
            for(auto &line: blockLines){
                for(auto it = line.begin(); it != line.end();){
                    if(it->life <= 0){
                        it = line.erase(it);
                    }else{
                        it++;
                    }
                }
            }

            // verifica colisoes com as paredes
            for(auto &wall: gameAreaWalls){
                auto wallCollision = ball.collider.intersects(wall);
                if(wallCollision.happened){
                    ball.velocity = ball.velocity.reflection(wallCollision.normal);
                }
            }

            // verifica se saiu p baixo
            for(auto it=balls.begin();it<balls.end();it++){
                if(it->position.y > (8 * (*screenHeight) / 9) + it->radius+5){
                    if(!firstBack){
                        firstBack = true;
                        it->velocity = Vector2::zero();
                        nextLaunchPosition = it->position;
                        nextLaunchPosition.y = 8 * (*screenHeight) / 9;
                    }else{
                        balls.erase(it);
                    }
                }
            }
        }

        // verifica se alguma bola saiu do campo. Se saiu, tenta corrigir
        for(auto &ball : balls){
            // left side
            if((ball.position.x+ball.radius) < 5 * (*screenWidth) / 14 + ball.radius){
                Vector2 normal = Vector2(1,0);
                ball.velocity = ball.velocity.reflection(normal);
                ball.position += normal*ball.collider.radius;
                continue;
            }
            // right side
            if((ball.position.x-ball.radius) > 9 * (*screenWidth) / 14 - ball.radius){
                Vector2 normal = Vector2(-1,0);
                ball.velocity = ball.velocity.reflection(normal);
                ball.position += normal*ball.collider.radius;
                continue;
            }
            // top side
            if((ball.position.y+ball.radius) < 1 * (*screenHeight) / 9 + ball.radius){
                Vector2 normal = Vector2(0,1);
                ball.velocity = ball.velocity.reflection(normal);
                ball.position += normal*ball.collider.radius;
                continue;
            }
        }

        //se alguma bola esta muito na horizontal
        // adicionamos uma componente vertical aleatoria
        // para acelerar e previnir blocks
        for(auto &ball: balls){
            bool up = rng()%2;
            while(fabs(ball.velocity * Vector2::right()) > 0.99f*ballSpeed){
                ball.velocity += (up?Vector2::up():Vector2::down())*ballSpeed*0.1f;
                // normaliza e multiplica para garantir que a bola nao acelerou
                ball.velocity.normalize();
                ball.velocity *= ballSpeed;
            }
        }

        if(balls.size() == 0){
            blockLines.push_back(createNewLine(level+1));
            pushLines();

            // checar game over(blocos na ultima linha!)
            for(auto &line : blockLines){
                for(auto &block : line){
                    if(block.position.y == blockAreaSize.y){
                        gameOver = true;
                        break;
                    }
                }
            }

            // verificar se passou o highscore
            if(level > PersistentStorage::get<int>("user", "highscore", 0)){
                PersistentStorage::set<int>("user", "highscore", level);
            }
            level++;

            // resetar variaveis p proximo nivel
            // constrain next launch position to be a valid one
            // in case the first ball went out of bounds
            if(nextLaunchPosition.x < 5 * (*screenWidth) / 14){
                nextLaunchPosition.x = 5 * (*screenWidth) / 14;
            }
            if(nextLaunchPosition.x > 9 * (*screenWidth) / 14){
                nextLaunchPosition.x = 9 * (*screenWidth) / 14;
            }

            ballLaunchPosition = nextLaunchPosition;
            balls.clear();
            firstBack = false;
            hasActivePlay = false;
            spawned = 0;
            burstCount ++; // TODO remover isso e fazer um sistema de powerups
        }
    }
}

void Game::render()
{
    CV::clear(Vector3::fromHex(0x232241));
    renderHeader();

    renderGameArea();
    particleManager.render();
}

void Game::renderHeader()
{
    Vector2 headerStart(5 * (*screenWidth) / 14, 0);
    Vector2 headerSize(4 * (*screenWidth) / 14, 1 * (*screenHeight) / 9);
    CV::translate(headerStart);
    CV::color(Vector3::fromHex(0xFFFFFF));

    // render pause button(fazer no app.cpp?)

    // render current score(fase)
    CV::text(headerSize * 0.5f,
        std::to_string(level),
        CustomFont::AgencyFB_Digits,
        Vector2(35,35),
        TextAlign::CENTER);

    // render coins and highscore
    int coins = PersistentStorage::get<int>("user", "coins", 0);
    int highscore = PersistentStorage::get<int>("user", "highscore", 0);
    auto coinObj = ObjLoader::get("coin");
    float lineHeight;
    FontManager::getLineHeight(CustomFont::AgencyFB_Digits, lineHeight);
    CV::color(Vector3::fromHex(0xFFD700));
    CV::obj(coinObj, Vector2(headerSize.x - 50, headerSize.y / 3), Vector2(7,-7));
    CV::text(Vector2(headerSize.x - 30, headerSize.y / 3 + (lineHeight*25)/2),
        std::to_string(coins),
        CustomFont::AgencyFB_Digits,
        Vector2(25,25),
        TextAlign::LEFT
    );

    auto trophyObj = ObjLoader::get("trophy");
    CV::color(Vector3::fromHex(0xFFFFFF));
    CV::obj(trophyObj, Vector2(headerSize.x - 50, 2 * headerSize.y / 3), Vector2(15,-15));
    CV::text(Vector2(headerSize.x - 30, 2 * headerSize.y / 3 + (lineHeight*25)/2),
        std::to_string(highscore),
        CustomFont::AgencyFB_Digits,
        Vector2(25,25),
        TextAlign::LEFT
    );
}

void Game::renderGameArea()
{
    Vector2 gameAreaStart(5 * (*screenWidth) / 14, 1 * (*screenHeight) / 9);
    Vector2 gameAreaSize(4 * (*screenWidth) / 14, 7 * (*screenHeight) / 9);

    // draw game area background
    CV::translate(gameAreaStart);
    CV::color(Vector3::fromHex(0x1D1E30));
    CV::rectFill(Vector2::zero(), gameAreaSize);
    CV::color(Vector3::fromHex(0xDD1111));
    CV::lineDashed( Vector2(gameAreaStart.x, gameAreaStart.y+gameAreaSize.y),
        Vector2(gameAreaStart.x+gameAreaSize.x, gameAreaStart.y+gameAreaSize.y),
        25, 2);

    for (auto &linha : blockLines)
    {
        renderLine(linha);
    }

    for (auto &powerup : powerups)
    {
        powerup.render();
    }

    if (hasActivePlay)
    {
        for (auto &ball : balls)
        {
            ball.render();
            #if PHYSICS_DEBUG
            CV::translate(0,0);
            // ball collider
            CV::color(1,0,0);
            CV::circle(ball.collider.position, 10, 16);
            // direction
            CV::color(0,0,1);
            CV::line(ball.collider.position, ball.collider.position+ball.velocity.normalized()*ball.collider.radius*4, 2.0f);
            // ball position
            CV::color(1,1,0);
            CV::circleFill(ball.position, 4, 8);
            CV::translate(gameAreaStart);
            #endif
        }

        // render dummy ball on launch position
        if(spawned < burstCount){
            Ball dummyBall;
            dummyBall.position = ballLaunchPosition;
            dummyBall.render();
        }
        if(firstBack){
            Ball dummyBall;
            dummyBall.position = nextLaunchPosition;
            dummyBall.render();
        }
    }
    else
    {
        renderArrow();

        // render dummy ball on launch position
        Ball dummyBall;
        dummyBall.position = ballLaunchPosition;
        dummyBall.render();
    }
}

void Game::renderLine(std::vector<Block> line)
{
    Vector2 gameAreaStart(5 * (*screenWidth) / 14, 1 * (*screenHeight) / 9);
    Vector2 gameAreaSize(4 * (*screenWidth) / 14, 7 * (*screenHeight) / 9);

    // descobrir tamanho maximo de bloco
    float blockSize = std::min(gameAreaSize.x / blockAreaSize.x, gameAreaSize.y / blockAreaSize.y);
    CV::translate(Vector2::zero());
    int blockmargin = 2;
    for (auto &block : line)
    {
        CV::color(block.color);
        Vector2 screenPos = gameAreaStart + block.position * blockSize;
        CV::rectFill(screenPos + Vector2(blockmargin),
            screenPos + Vector2(blockSize) - Vector2(blockmargin));
        CV::color(Vector3::fromHex(0xFFFFFF));
        CV::text(screenPos.x + blockSize * 0.5f,
            screenPos.y + blockSize * 0.5f + 0.9f*CV::fontHeight(GLUT_BITMAP_HELVETICA_18)/2,
            std::to_string(block.life).c_str(),
            GLUT_BITMAP_HELVETICA_18,
            TextAlign::CENTER);

        #if PHYSICS_DEBUG
        // render collider
        CV::color(0,1,0);
        CV::rect(block.collider.position, block.collider.position+block.collider.size);
        #endif
    }
}

void Game::renderArrow()
{
    Vector2 gameAreaStart(5 * (*screenWidth) / 14, 1 * (*screenHeight) / 9);
    Vector2 gameAreaSize(4 * (*screenWidth) / 14, 7 * (*screenHeight) / 9);
    CV::translate(gameAreaStart);

    Vector2 finalPosition = mousePos - gameAreaStart;
    finalPosition.x = std::max(0.0f, std::min(gameAreaSize.x, finalPosition.x));
    finalPosition.y = std::max(0.0f, std::min(gameAreaSize.y, finalPosition.y));

    CV::color(Vector3::fromHex(0x00FF00));
    CV::line(ballLaunchPosition - gameAreaStart, finalPosition, 3.0f);
}

void Game::keyDown(Key key)
{
    if(key == 32){
        if(hasActivePlay){
            ballSpeedMultiplier = 2.0f;
        }
    }
}

void Game::keyUp(Key key){
    if(key == 32){
        ballSpeedMultiplier = 1.0f;
    }
}

void Game::mouseUp()
{
}

void Game::mouseDown()
{
    if (!hasActivePlay)
    {
        hasActivePlay = true;
        balls.clear();
        ballLaunchDirection = (mousePos - ballLaunchPosition).normalized();
    }
}

void Game::updateMousePos(Vector2 pos)
{
    mousePos = pos;
}

std::vector<Block> Game::createNewLine(int level)
{
    std::uniform_int_distribution<int> blockCountRng(1, 6);
    std::uniform_int_distribution<int> candidateRng(0, blockAreaSize.x - 1);

    Vector3 lineColor = blockColors[level % blockColors.size()];

    int numBlocks = blockCountRng(rng);
    int blockLife = level;
    if (level % 10 == 0)
    {
        blockLife *= 2;
    }

    std::vector<Block> blocks;
    bool *bPos = new bool[(int)blockAreaSize.x]{false};
    for (int i = 0; i < numBlocks; i++)
    { // achar posicao de cada bloco
        int candidateIndex;
        do
        {
            candidateIndex = candidateRng(rng);
        } while (bPos[candidateIndex] == true);
        bPos[candidateIndex] = true;

        Block b(Vector2(candidateIndex, 1),
                lineColor,
                blockLife);
        b.collider.id = rng();
        blocks.push_back(b);
    }
    // mais uma passada pra garantir q n tem id 0
    for(auto &block: blocks){
        if(block.collider.id == 0){
            block.collider.id = rng();
            #if PHYSICS_DEBUG
            std::cout << "Regenerating block collider id" << std::endl;
            #endif
        }
    }
    return blocks;
}

void Game::pushLines(){
    for(auto &line: blockLines){
        for(auto &block: line){
            block.position.y++;
        }
    }
}

void Game::updateGameAreaBounds(){

    if(gameAreaWalls.size() < 3){
        gameAreaWalls.clear();
        auto left = Rectangle2D();
        left.id = rng();
        gameAreaWalls.push_back(left);
        auto right = Rectangle2D();
        right.id = rng();
        gameAreaWalls.push_back(right);
        auto ceiling = Rectangle2D();
        ceiling.id = rng();
        gameAreaWalls.push_back(ceiling);
    }
    // left wall
    gameAreaWalls[0].position = Vector2(5 * (*screenWidth) / 14, 1 * (*screenHeight) / 9);
    gameAreaWalls[0].size = Vector2(1, 7 * (*screenHeight) / 9);

    // right wall
    gameAreaWalls[1].position = Vector2(9 * (*screenWidth) / 14, 1 * (*screenHeight) / 9);
    gameAreaWalls[1].size = Vector2(1, 7 * (*screenHeight) / 9);

    // ceiling
    gameAreaWalls[2].position = Vector2(5 * (*screenWidth) / 14, 1 * (*screenHeight) / 9);
    gameAreaWalls[2].size = Vector2(4 * (*screenWidth) / 14, 1);

    if(gameAreaWalls[0].id == 0 || gameAreaWalls[1].id == 0 || gameAreaWalls[2].id == 0){
        gameAreaWalls[0].id = rng();
        gameAreaWalls[1].id = rng();
        gameAreaWalls[2].id = rng();
    }
}

bool Game::isGameOver(){
    return gameOver;
}

Block* Game::getBlockAt(Vector2 pos){
    for(auto &line: blockLines){
        for(auto &block: line){
            if(block.position == pos){
                return &block;
            }
        }
    }
    return nullptr;
}

void hit(Block& block, ParticleManager& mng){
    block.life--;

    if(block.life <= 0){
        SoundPlayer::play("ballHit");
        mng.spawn(
            ObjLoader::get("star"),//modelo
            20,//qtd
            block.collider.position + Vector2(block.collider.size.x/2, block.collider.size.y/2), //pos
            Vector2(5,5), // escala
            {
                Vector3::fromHex(0xd62828),
                Vector3::fromHex(0xf77f00),
                Vector3::fromHex(0xfcbf49),
                Vector3::fromHex(0xffbe0b),
                Vector3::fromHex(0xff7d00),
                Vector3::fromHex(0xf18701)
            }, // cores
            1.0f, //ttl
            100.0f, //forca
            true//gravidade
        );
    }
}

bool Game::processCollisions(Ball& ball, Block& block){
    if(block.life <= 0){
        return false;
    }
    Block* blockEsq = getBlockAt(block.position + Vector2(-1,0));
    Block* blockDir = getBlockAt(block.position + Vector2(1,0));

    Collision colisaoCentral = ball.collider.intersects(block.collider);
    bool colidiuCentro = colisaoCentral.happened;
    bool colidiuEsq = blockEsq != nullptr ? ball.collider.intersects(blockEsq->collider).happened : false;
    bool colidiuDir = blockDir != nullptr ? ball.collider.intersects(blockDir->collider).happened : false;

    if(colidiuCentro && !colidiuEsq && !colidiuDir){ // isso funciona
        // colisao normal no meio do bloco
        ball.velocity = ball.velocity.reflection(colisaoCentral.normal);
        hit(block, particleManager);
        return true;
    }else if(colidiuCentro && (colidiuEsq || colidiuDir)){ // isso funciona
        // colisao entre 2 blocos, normal é simulada com um plano
        Vector2 normal = Vector2(0,1);
        ball.velocity = ball.velocity.reflection(normal);
        hit(block, particleManager);
        if(colidiuEsq && blockEsq != nullptr){
            hit(*blockEsq, particleManager);
        }
        if(colidiuDir && blockDir != nullptr){
            hit(*blockDir, particleManager);
        }
        return true;
    }else if(block.collider.pointInside(ball.position)){ // isso nao kkk
        auto col = ball.collider.intersects(block.collider, true);

        if(col.happened){
            ball.velocity = ball.velocity.reflection(col.normal);
            hit(block, particleManager);
            return true;
        }
        return false;
    }else{
        return false;
    }
}
