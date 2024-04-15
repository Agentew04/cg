#include "App.h"

#include <random>

#include "../gl_canvas2d.h"
#include "../Storage/PersistentStorage.h"
#include "../3D/ObjLoader.h"

App::App(int *scrW, int *scrH)
    :
    currentMenu(MenuState::MAIN_MENU),
    screenWidth(scrW), screenHeight(scrH),
    mousePos(0,0),
    game(scrW, scrH)
{
    submitButtons();

    // create persistent unique user id
    if(!PersistentStorage::has("user","uniqueid")){
        std::random_device r;
        std::mt19937 rgen(r());
        int uniqueId = rgen();
        PersistentStorage::set<int>("user","uniqueid",uniqueId);
    }

    // load user data
    coins = PersistentStorage::getOrSetDefault<int>("user","coins", 0);
    highscore = PersistentStorage::getOrSetDefault<int>("user","highscore", 0);

    username = PersistentStorage::getOrSetDefault<std::string>("user","name", "");
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

void App::keyDown(Key key){
    switch(currentMenu){
    case MenuState::IDENTIFICATION:
        idTextBox->keyDown(key);
    case MenuState::GAME:
        game.keyDown(key);
        break;
    default:
        break;
    }
}

void App::mouseUp()
{
    switch (currentMenu)
    {
    case MenuState::MAIN_MENU:
        mainMenuButtons.mouseUp();
        break;
    case MenuState::IDENTIFICATION:
        idTextBox->mouseUp();
        idButtons.mouseUp();
        break;
    case MenuState::PAUSED:
        pauseButtons.mouseUp();
        break;
    case MenuState::GAME:
        gameButtons.mouseUp();
        game.mouseUp();
        break;
    case MenuState::GAME_OVER:
        gameOverButtons.mouseUp();
        break;
    case MenuState::POST_GAME_STATS:
        postGameStatsButtons.mouseUp();
        break;
    }
}

void App::mouseDown()
{
    switch (currentMenu)
    {
    case MenuState::MAIN_MENU:
        mainMenuButtons.mouseDown();
        break;
    case MenuState::IDENTIFICATION:
        idTextBox->mouseDown();
        idButtons.mouseDown();
        break;
    case MenuState::PAUSED:
        pauseButtons.mouseDown();
        break;
    case MenuState::GAME:
        gameButtons.mouseDown();
        game.mouseDown();
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
    case MenuState::IDENTIFICATION:
        idTextBox->updateMousePos(pos);
        idButtons.updateMousePos(pos);
        break;
    case MenuState::PAUSED:
        pauseButtons.updateMousePos(pos);
        break;
    case MenuState::GAME:
        gameButtons.updateMousePos(pos);
        game.updateMousePos(pos);
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
    case MenuState::IDENTIFICATION:
        renderIdentification();
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
    CV::color(Vector3::fromHex(0xFFFFFF));

    CV::translate((*screenWidth)/2, 2.75*(*screenHeight)/11);
    ObjLoader::get("logo")->scale = Vector3(75,-75,0);
    CV::obj(ObjLoader::get("logo"));
    // CV::text((*screenWidth)/2, 3*(*screenHeight)/11,
    //     "BALL BOUNCE",
    //     GLUT_BITMAP_TIMES_ROMAN_24,
    //     TextAlign::CENTER
    // );
    CV::translate(0,0);
    CV::text((*screenWidth)/2, 5*(*screenHeight)/11,
        ("Coins: " + std::to_string(coins)).c_str(),
        GLUT_BITMAP_HELVETICA_18,
        TextAlign::CENTER
    );
    mainMenuButtons.draw();
}

void App::renderIdentification(){
    CV::clear(Vector3::fromHex(0x1D1E30));

    idTextBox->render();
    idButtons.draw();
}

void App::renderGame()
{
    game.render();
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
    // mainmenu
    auto mainmenuPlay = new Button(
        [&](){
            return Vector2((*screenWidth)/2, 6.5*(*screenHeight)/11);
        },
        [](){
            return Vector2(200, 50);
        },
        UIPlacement::CENTER,
        "Play",
        [this](Button*){
            std::cout << "Play button clicked" << std::endl;
            if(username == ""){
                currentMenu = MenuState::IDENTIFICATION;
            }else{
                currentMenu = MenuState::GAME;
            }
        }
    );
    mainmenuPlay->style = ButtonStyle::FlatLightBlue();
    mainMenuButtons.registerButton(mainmenuPlay);


    idTextBox = new TextBox(
        [&](){
            return Vector2((*screenWidth)/2, 5*(*screenHeight)/11);
        },
        [](){
            return Vector2(200, 50);
        },
        UIPlacement::CENTER,
        TextAlign::CENTER
    );
    idTextBox->style = TextBox::Style::FlatWhite();

    auto idPlay = new Button(
        [&](){
            return Vector2((*screenWidth)/2, 7*(*screenHeight)/11);
        },
        [](){
            return Vector2(200, 50);
        },
        UIPlacement::CENTER,
        "Play",
        [this](Button*){
            if(idTextBox->getText() == ""){
                return;
            }
            PersistentStorage::set<std::string>("user","name",idTextBox->getText());
            std::cout << "Play button clicked" << std::endl;
            currentMenu = MenuState::GAME;
        }
    );
    idPlay->style = ButtonStyle::FlatGreen();
    idButtons.registerButton(idPlay);
}

// ENDREGION UI
