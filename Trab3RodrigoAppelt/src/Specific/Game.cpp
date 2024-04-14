#include "Game.h"

#include <string>
#include <functional>
#include <random>

#include "../gl_canvas2d.h"
#include "../Storage/PersistentStorage.h"

Game::Game(int *scrW, int *scrH) : screenWidth(scrW), screenHeight(scrH),
                                   mousePos(Vector2::zero()),
                                   hasActivePlay(false),
                                   burstCount(5),
                                   spawned(0),
                                   lastBurstTime(0.0f),
                                   level(1),
                                   firstBack(false),
                                   ballLaunchPosition((*scrW) / 2, (*scrH) - 100),
                                   ballLaunchDirection(Vector2::zero()),
                                   nextLaunchPosition(Vector2::zero()),
                                   ballSpeed(200.0f)
{
    // std::random_device r; <- versao 8.1.0 do g++ do codeblocks n suporta
    //                          passar a seed antiga direto pro mersene
    rng = std::mt19937(time(nullptr));
    std::cout << std::to_string(rng()) << " " << std::to_string(rng()) << " " << std::to_string(rng()) << std::endl;
    blockLines.push_back(createNewLine(1));
    pushLines();
    blockLines.push_back(createNewLine(2));
    pushLines();
    blockLines.push_back(createNewLine(3));

}

Game::~Game()
{
}

void Game::reset()
{
    hasActivePlay = false;
    burstCount = 1;
    spawned = 0;
    level = 1;
    firstBack = false;
    lastBurstTime = 0.0f;
    ballLaunchPosition = Vector2((*screenWidth) / 2, 1 * (*screenHeight) / 9);
    ballLaunchDirection = Vector2::zero();
    firstBack = false;
}

void Game::update(float delta)
{
    if (hasActivePlay)
    {
        // dispara nova bola na rajada se possivel
        if(burstCount > spawned && CV::time() > lastBurstTime + burstInterval){
            Ball b;
            b.position = ballLaunchPosition;
            b.velocity = ballLaunchDirection*ballSpeed;
            b.collider = Circle2D(b.position, b.radius);
            balls.push_back(b);
            spawned++;
            lastBurstTime = CV::time();
        }

        // atualiza os colisores
        {
            Vector2 gameAreaStart(5 * (*screenWidth) / 14, 1 * (*screenHeight) / 9);
            Vector2 gameAreaSize(4 * (*screenWidth) / 14, 7 * (*screenHeight) / 9);
            float blockSize = std::min(gameAreaSize.x / blockAreaSize.x, gameAreaSize.y / blockAreaSize.y);

            for(auto &line: blockLines){
                for(auto &block: line){
                    block.collider = Rectangle2D(
                        gameAreaStart + block.position*blockSize, 
                        Vector2(blockSize, blockSize));
                }
            }
            for(auto &ball: balls){
                ball.collider = Circle2D(ball.position, ball.radius);
            }
        }
            

        //move bolinhas
        for(auto &ball: balls){
            ball.position += ball.velocity*delta;
        }
        
        //verifica colisoes
        for(auto &ball: balls){
            // verifica colisoes com os blocos
            for(auto &line: blockLines){
                // iterator for para ir deletando conforme quebram
                for(auto it = line.begin(); it<line.end(); it++){
                    // se bateu em algum bloco
                    auto blockCollision = ball.collider.intersects(it->collider);
                    if(blockCollision.happened){
                        it->life--;
                        if(it->life <= 0){
                            line.erase(it);
                        }

                        // reflete bola
                        ball.velocity = ball.velocity.reflection(blockCollision.normal);
                    }
                }
            }
        
            // verifica colisoes com as paredes
            for(auto &wall: getGameAreaBounds()){
                auto wallCollision = ball.collider.intersects(wall);
                if(wallCollision.happened){
                    ball.velocity = ball.velocity.reflection(wallCollision.normal);
                }
            }

            // verifica se saiu p baixo
            for(auto it=balls.begin();it<balls.end();it++){
                if(it->position.y > (8 * (*screenHeight) / 9) + it->radius+5){
                    std::cout << "saiu p baixo" << std::endl;
                    if(!firstBack){
                        firstBack = true;
                        it->velocity = Vector2::zero();
                        nextLaunchPosition = it->position;
                        nextLaunchPosition.y = 8 * (*screenHeight) / 9;
                        std::cout << "first back" << std::endl;
                    }else{
                        std::cout << "deleted" << std::endl;
                        balls.erase(it);
                    }
                }
            }

            // eventualmente, colisao com powerup bounce
        }

        if(balls.size() == 0){
            pushLines();
            blockLines.push_back(createNewLine(level));
            ballLaunchPosition = nextLaunchPosition;
            balls.clear();
            level++;
            spawned = 0;
            firstBack = false;
            hasActivePlay = false;

            // checa se deu game over
        }
    }
}

void Game::render()
{
    CV::clear(Vector3::fromHex(0x232241));
    renderHeader();

    renderGameArea();
}

void Game::renderHeader()
{
    Vector2 headerStart(5 * (*screenWidth) / 14, 0);
    Vector2 headerSize(4 * (*screenWidth) / 14, 1 * (*screenHeight) / 9);
    CV::translate(headerStart);

    // render pause button(fazer no app.cpp?)

    // render current score(fase)
    CV::text(headerSize.x / 2, headerSize.y / 2, std::to_string(level).c_str(), GLUT_BITMAP_HELVETICA_18, TextAlign::CENTER);
    // render coins and highscore
    int coins = PersistentStorage::get<int>("user", "coins", 0);
    int highscore = PersistentStorage::get<int>("user", "highscore", 0);
    CV::text(headerSize.x, headerSize.y / 3,
             ("Coins: " + std::to_string(coins)).c_str(),
             GLUT_BITMAP_HELVETICA_18, TextAlign::RIGHT);
    CV::text(headerSize.x, 2 * headerSize.y / 3,
             ("Highscore: " + std::to_string(highscore)).c_str(),
             GLUT_BITMAP_HELVETICA_18, TextAlign::RIGHT);
}

void Game::renderGameArea()
{
    Vector2 gameAreaStart(5 * (*screenWidth) / 14, 1 * (*screenHeight) / 9);
    Vector2 gameAreaSize(4 * (*screenWidth) / 14, 7 * (*screenHeight) / 9);

    // draw game area background
    CV::translate(gameAreaStart);
    CV::color(Vector3::fromHex(0x1D1E30));
    CV::rectFill(Vector2::zero(), gameAreaSize);

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

        // render collider
        CV::color(0,1,0);
        CV::rect(block.collider.position, block.collider.position+block.collider.size);
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
                Vector3::fromHex(0xAA3333),
                blockLife);
        blocks.push_back(b);
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

std::vector<Rectangle2D> Game::getGameAreaBounds(){
    std::vector<Rectangle2D> bounds;

    // left wall
    bounds.push_back(Rectangle2D(Vector2(5 * (*screenWidth) / 14, 1 * (*screenHeight) / 9),
                                  Vector2(1, 7 * (*screenHeight) / 9)));

    // right wall
    bounds.push_back(Rectangle2D(Vector2(9 * (*screenWidth) / 14, 1 * (*screenHeight) / 9),
                                  Vector2(1, 7 * (*screenHeight) / 9)));

    // ceiling
    bounds.push_back(Rectangle2D(Vector2(5 * (*screenWidth) / 14, 1 * (*screenHeight) / 9),
                                  Vector2(4 * (*screenWidth) / 14, 1)));
    
    //floor
    //bounds.push_back(Rectangle2D(Vector2(5 * (*screenWidth) / 14, 8 * (*screenHeight) / 9),
    //                              Vector2(4 * (*screenWidth) / 14, 1)));
    return bounds;
}