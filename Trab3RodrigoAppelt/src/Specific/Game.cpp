#include "Game.h"

#include <string>

#include "../gl_canvas2d.h"
#include "../Storage/PersistentStorage.h"

Game::Game(int *scrW, int *scrH) :
    screenWidth(scrW), screenHeight(scrH),
    mousePos(0,0),
    hasActivePlay(false),
    burstCount(1),
    level(1),
    ballLaunchPosition((*scrW)/2, (*scrH)-100)
{
}

Game::~Game()
{
}

void Game::reset(){
    hasActivePlay = false;
    burstCount = 1;
    level = 1;
    ballLaunchPosition = Vector2((*screenWidth)/2, 1*(*screenHeight)/9);
}

void Game::update(float delta)
{
    if(hasActivePlay){
    //     se ainda tem bolinhas na rajada e ja passou o delay
    //         spawna bola nova
    //     move bolinhas
    //     verifica colisoes e suas reacoes
    //     se todas bolas bateram em baixo
    //         desce blocos
    //         add nova linha de blocos
    //         hasActivePlay = false
    }else{
    //     desenha seta ate mouse(direcao)
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
    Vector2 headerStart(5*(*screenWidth)/14, 0);
    Vector2 headerSize(4*(*screenWidth)/14, 1*(*screenHeight)/9);
    CV::translate(headerStart);

    // render pause button(fazer no app.cpp?)
    
    // render current score(fase)
    CV::text(headerSize.x/2, headerSize.y/2, std::to_string(level).c_str(), GLUT_BITMAP_HELVETICA_18, TextAlign::CENTER);
    // render coins and highscore
    int coins = PersistentStorage::get<int>("user", "coins", 0);
    int highscore = PersistentStorage::get<int>("user", "highscore", 0);
    CV::text(headerSize.x, headerSize.y/3, 
        ("Coins: " + std::to_string(coins)).c_str(), 
        GLUT_BITMAP_HELVETICA_18, TextAlign::RIGHT
    );
    CV::text(headerSize.x, 2*headerSize.y/3, 
        ("Highscore: " + std::to_string(highscore)).c_str(), 
        GLUT_BITMAP_HELVETICA_18, TextAlign::RIGHT
    );
}

void Game::renderGameArea(){
    Vector2 gameAreaStart(5*(*screenWidth)/14, 1*(*screenHeight)/9);
    Vector2 gameAreaSize(4*(*screenWidth)/14, 7*(*screenHeight)/9);

    // draw game area background
    CV::translate(gameAreaStart);
    CV::color(Vector3::fromHex(0x1D1E30));
    CV::rectFill(Vector2::zero(), gameAreaSize);

    for(auto &linha: blockLines){
        for(auto &block : linha){
            block.render();
        }
    }

    for(auto &powerup: powerups){
        powerup.render();
    }

    if(hasActivePlay){
        for(auto &ball: balls){
            ball.render();
        }
    }else{
        renderArrow();
        
        // render dummy ball on launch position
        Ball dummyBall;
        dummyBall.position = ballLaunchPosition;
        dummyBall.render();
    }
}

void Game::renderArrow()
{
    Vector2 gameAreaStart(5*(*screenWidth)/14, 1*(*screenHeight)/9);
    Vector2 gameAreaSize(4*(*screenWidth)/14, 7*(*screenHeight)/9);
    CV::translate(gameAreaStart);

    Vector2 finalPosition = mousePos-gameAreaStart;
    finalPosition.x = std::max(0.0f, std::min(gameAreaSize.x, finalPosition.x));
    finalPosition.y = std::max(0.0f, std::min(gameAreaSize.y, finalPosition.y));
    
    CV::color(Vector3::fromHex(0x00FF00));
    CV::line(ballLaunchPosition-gameAreaStart, finalPosition, 3.0f);
}

void Game::keyDown(Key key){

}

void Game::mouseUp()
{
}

void Game::mouseDown()
{
}

void Game::updateMousePos(Vector2 pos)
{
    mousePos = pos;
}
