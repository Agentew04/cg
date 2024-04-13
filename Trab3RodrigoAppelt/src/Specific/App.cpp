#include "App.h"

#include "../gl_canvas2d.h"
#include "../Storage/PersistentStorage.h"

App::App(int *scrW, int *scrH)
    : 
    currentMenu(MenuState::MAIN_MENU),
    screenWidth(scrW), screenHeight(scrH),
    mousePos(0,0)
{
    submitButtons();

    // create persistent unique user id
    if(!PersistentStorage::hasInt("user","uniqueid")){
        PersistentStorage::setInt("user","uniqueid",rand());
    }
}

App::~App()
{
}

void App::update(float delta)
{
    if(currentMenu == MenuState::GAME){
        game.update(delta);
    }
}

void App::mouseUp()
{
}

void App::mouseDown()
{
    switch (currentMenu)
    {
    case MenuState::MAIN_MENU:
        mainMenuButtons.mouseDown();
        break;
    case MenuState::PAUSED:
        pauseButtons.mouseDown();
        break;
    case MenuState::GAME:
        gameButtons.mouseDown();
        break;
    case MenuState::GAME_OVER:
        gameOverButtons.mouseDown();
        break;
    case MenuState::POST_GAME_STATS:
        postGameStatsButtons.mouseDown();
        break;
    }
}

void App::updateMousePos(Vector2 pos)
{
    mousePos = pos;
    switch (currentMenu)
    {
    case MenuState::MAIN_MENU:
        mainMenuButtons.updateMousePos(pos);
        break;
    case MenuState::PAUSED:
        pauseButtons.updateMousePos(pos);
        break;
    case MenuState::GAME:
        gameButtons.updateMousePos(pos);
        break;
    case MenuState::GAME_OVER:
        gameOverButtons.updateMousePos(pos);
        break;
    case MenuState::POST_GAME_STATS:
        postGameStatsButtons.updateMousePos(pos);
        break;
    }
}

// REGION RENDER

void App::render()
{
    switch (currentMenu)
    {
    case MenuState::MAIN_MENU:
        renderMainMenu();
        break;
    case MenuState::PAUSED:
    case MenuState::GAME:
        renderGame();
        if(currentMenu == MenuState::PAUSED){
            renderPauseMenu(); // overlay
        }
        break;
    case MenuState::GAME_OVER:
        renderGameOver();
        break;
    case MenuState::POST_GAME_STATS:
        // draw end game stats, pb, etc
        renderPostGameStats();
        break;
    }
}

void App::renderMainMenu()
{
    CV::clear(Vector3::fromHex(0x1D1E30));

    CV::text((*screenWidth)/2, 3*(*screenHeight)/11, 
        "BALL BOUNCE", 
        GLUT_BITMAP_TIMES_ROMAN_24,
        TextAlign::CENTER
    );
    mainMenuButtons.draw();
}

void App::renderGame()
{
    // draw game
}

void App::renderPauseMenu()
{
    // draw pause menu
}

void App::renderGameOver()
{
    // draw game over
}

void App::renderPostGameStats()
{
    // draw post game stats
}


// ENDREGION RENDER

// REGION UI

void App::submitButtons(){
    // render
}

// ENDREGION UI