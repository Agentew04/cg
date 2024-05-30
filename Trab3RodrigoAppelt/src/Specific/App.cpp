#include "App.h"

#include <random>
#include <ctime>

#include "../gl_canvas2d.h"
#include "../Storage/PersistentStorage.h"
#include "../3D/ObjLoader.h"
#include "../Web/WebManager.h"
#include "Store.h"

App::App(int *scrW, int *scrH)
    :
    currentMenu(MenuState::IDENTIFICATION),
    screenWidth(scrW), screenHeight(scrH),
    mousePos(0,0),
    game(scrW, scrH),
    store(scrW, scrH)
{
    submitButtons();

    // create persistent unique user id
    if(!PersistentStorage::has("user","uniqueid")){
        std::mt19937 rgen(time(nullptr));
        int uniqueId = rgen();
        PersistentStorage::set<int>("user","uniqueid",uniqueId);
    }

    // load user data
    username = PersistentStorage::getOrSetDefault<std::string>("user","name", "");
    PersistentStorage::setIfNot("user","coins",0);
    PersistentStorage::setIfNot("user","highscore",0);
    store.load();
}

App::~App()
{
}

void App::update(float delta)
{
    switch (currentMenu)
    {
    case MenuState::IDENTIFICATION:
        if(username != ""){
            currentMenu = MenuState::MAIN_MENU;
        }
        break;
    case MenuState::GAME:
        game.update(delta);
        if(game.isGameOver()){
            // primeiro frame que ele perdeu.
            // enviar o highscore para servidor
            bool success = false;
            PersistentStorage::set("user", "highscore", game.getScore()); // vai sobrepor oq o game escreveu mas azar
            PersistentStorage::set("game", "hasSavedGame", 0);
            WebManager::post(
                "ballsbounce.rodrigoappelt.com",
                "/highscores",
                {
                    {"name", username},
                    {"score", std::to_string(game.getScore())},
                    {"userId", std::to_string(PersistentStorage::get<int>("user","uniqueid",0))}
                },
                &success
            );

            if(!success){
                // nao consegui enviar pro server
            }

            // fetch highscores
            success = false;
            auto res = WebManager::get(
                "ballsbounce.rodrigoappelt.com",
                "/highscores",
                {
                    { "maxEntries", "10"}
                },
                &success
            );
            if(success){
                highscoresBuffer.clear();
                auto arr = WebManager::fromJsonArray(res);
                for(auto obj : arr){
                    highscoresBuffer.push_back({obj["name"], std::stoi(obj["score"])});
                }
                highscoresFetched = true;
            }

            currentMenu = MenuState::GAME_OVER;
        }
        break;
    case MenuState::STORE:
        if(store.wantToGoBack){
            currentMenu = MenuState::MAIN_MENU;
            store.wantToGoBack = false;
        }
        break;
    default:
        break;
    }
}

void App::keyDown(Key key){
    switch(currentMenu){
    case MenuState::IDENTIFICATION:
        idTextBox->keyDown(key);
        break;
    case MenuState::GAME:
        if(key == ESC){
            currentMenu = MenuState::PAUSED;
        }else{
            game.keyDown(key);
        }
        break;
    case MenuState::PAUSED:
        if(key == ESC){
            currentMenu = MenuState::GAME;
        }
        break;
    case MenuState::HIGHSCORES:
        if(key == ESC){
            currentMenu = MenuState::MAIN_MENU;
        }
        break;
    case MenuState::STORE:
        if(key == ESC){
            currentMenu = MenuState::MAIN_MENU;
        }
        break;
    default:
        break;
    }
}

void App::keyUp(Key key){
    switch(currentMenu){
    case MenuState::IDENTIFICATION:
        // idTextBox->keyUp(key);
        break;
    case MenuState::GAME:
        game.keyUp(key);
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
    case MenuState::HIGHSCORES:
        highscoresButtons.mouseUp();
        break;
    case MenuState::STORE:
        store.mouseUp();
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
    case MenuState::HIGHSCORES:
        highscoresButtons.mouseDown();
        break;
    case MenuState::STORE:
        store.mouseDown();
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
    case MenuState::HIGHSCORES:
        highscoresButtons.updateMousePos(pos);
        break;
    case MenuState::STORE:
        store.updateMousePos(pos);
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
    case MenuState::HIGHSCORES:
        renderHighscores();
        break;
    case MenuState::STORE:
        renderStore();
        break;
    }
}

void App::renderMainMenu()
{
    CV::clear(Vector3::fromHex(0x1D1E30));
    CV::color(Vector3::fromHex(0xFFFFFF));


    CV::translate(0,0);
    CV::obj(ObjLoader::get("logo"),
        Vector2((*screenWidth)/2, 2*(*screenHeight)/11),
        Vector2(75,75));
    int coins = PersistentStorage::getOrSetDefault<int>("user","coins",0);
    CV::text(Vector2(
            (*screenWidth)/2, 4.2*(*screenHeight)/11
        ),
        "Moedas: " + std::to_string(coins),
        20
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
    // draw semi transparent background
    CV::translate(Vector2::zero());
    CV::color(Vector3::fromHex(0x000000), 0.5f);
    CV::rectFill(Vector2::zero(), Vector2(*screenWidth, *screenHeight));

    // draw PAUSED title
    CV::color(Vector3::fromHex(0xFFFFFF));
    CV::text(Vector2(
            *screenWidth/2, *screenHeight/6
        ),
        "PAUSED",
        25
    );

    // render buttons
    pauseButtons.draw();
}

void App::renderGameOver()
{
    // draw post game screen
    CV::clear(Vector3::fromHex(0x1D1E30));

    // draw left panel
    CV::translate(Vector2::zero());
    CV::color(Vector3::fromHex(0x242535)); // shadow
    CV::rectFill(Vector2(1* *screenWidth/9+15, 1* *screenHeight/7+15), Vector2(4* *screenWidth/9+15, 6* *screenHeight/7+15));
    CV::color(Vector3::fromHex(0x383947)); // bg
    CV::rectFill(Vector2(1* *screenWidth/9, 1* *screenHeight/7), Vector2(4* *screenWidth/9, 6* *screenHeight/7));
    CV::translate(Vector2(1* *screenWidth/9, 1* *screenHeight/7));
    // draw game over and stats
    CV::color(Vector3::fromHex(0xFFFFFF));
    CV::text(Vector2(
            1.5* *screenWidth/9, 0.5* *screenHeight/7
        ),
        "GAME OVER",
        25
    );
    CV::text(Vector2(
            1.5* *screenWidth/9, 2* *screenHeight/7
        ),
        ("Score: " + std::to_string(game.getScore())),
        25
    );
    CV::text(Vector2(
            1.5* *screenWidth/9, 2.5* *screenHeight/7
        ),
        ("Highscore: " + std::to_string(PersistentStorage::get<int>("user", "highscore",0))),
        25
    );


    // draw right panel
    CV::translate(Vector2::zero());
    CV::color(Vector3::fromHex(0x242535));
    CV::rectFill(Vector2(5* *screenWidth/9+15, 1* *screenHeight/7+15), Vector2(8* *screenWidth/9+15, 6* *screenHeight/7+15));
    CV::color(Vector3::fromHex(0x383947));
    CV::rectFill(Vector2(5* *screenWidth/9, 1* *screenHeight/7), Vector2(8* *screenWidth/9, 6* *screenHeight/7));
    CV::translate(Vector2(5* *screenWidth/9, 1* *screenHeight/7));
    // draw highscores panel
    CV::color(Vector3::fromHex(0xFFFFFF));
    CV::text(Vector2(
            1.5* *screenWidth/9, 0.5* *screenHeight/7
        ),
        "Placar de Lideres",
        25
    );
    if(!highscoresFetched){
        CV::text(Vector2(
                1.5* *screenWidth/9, 3* *screenHeight/7
            ),
            "Nao foi possivel carregar\nEsta sem internet?",
            25
        );
    }else{
        // draw highscore list
        int i = 0;
        for(auto score : highscoresBuffer){
            std::string line = std::to_string(i+1) + ". " + score.first + ": " + std::to_string(score.second);
            CV::text(Vector2(
                    1* *screenWidth/9, (0.5+((i+1)/2.0))* *screenHeight/7
                ), //comeca em 1, sobe 0.5 a cada linha
                line,
                25,
                FontName::JetBrainsMono,
                UIPlacement::LEFT
            );
            i++;
        }
    }

    // draw buttons
    gameOverButtons.draw();
}

void App::renderHighscores(){
    CV::clear(Vector3::fromHex(0x1D1E30));

    // draw highscores
    CV::color(Vector3::fromHex(0xFFFFFF));
    CV::text(Vector2(
            *screenWidth/2, *screenHeight/6
        ),
        "Placar de Lideres",
        20
    );

    if(!highscoresFetched){
        CV::text(Vector2(
                *screenWidth/2, 3* *screenHeight/6
            ),
            "Nao foi possivel carregar\nEsta sem internet?",
            20
        );
    }else{
        // draw highscore list
        int i = 0;
        for(auto score : highscoresBuffer){
            std::string line = std::to_string(i+1) + ". " + score.first + ": " + std::to_string(score.second);
            CV::text(Vector2(
                    *screenWidth/2, (0.5+((i+2)/2.0))* *screenHeight/6
                ), //comeca em 1, sobe 0.5 a cada linha
                line,
                20
            );
            i++;
        }
    }

    // draw buttons
    highscoresButtons.draw();
}

void App::renderStore(){
    store.render();
}

// ENDREGION RENDER

// REGION UI

void App::submitButtons(){
    // mainmenu
    auto mainmenuPlay = new Button(
        [&](){
            return Vector2((*screenWidth)/2, 5*(*screenHeight)/11);
        },
        [](){
            return Vector2(200, 50);
        },
        UIPlacement::CENTER,
        "Jogar",
        [this](Button*){
            if(username == ""){
                currentMenu = MenuState::IDENTIFICATION;
            }else{
                currentMenu = MenuState::GAME;
            }
        }
    );
    mainmenuPlay->style = ButtonStyle::FlatLightBlue();
    mainMenuButtons.registerButton(mainmenuPlay);

    auto mainmenuContinue = new Button(
        [&](){
            return Vector2((*screenWidth)/2, 6.5*(*screenHeight)/11);
        },
        [](){
            return Vector2(200, 50);
        },
        UIPlacement::CENTER,
        "Continuar",
        [this](Button*){
            // checar se realmente tem jogo salvo
            if(PersistentStorage::get("game", "hasSavedGame", 0) == 1){
                game.loadState();
                currentMenu = MenuState::GAME;
            }
        }
    );
    mainmenuContinue->style = ButtonStyle::FlatLightBlue();
    mainMenuButtons.registerButton(mainmenuContinue);

    auto mainmenuhighscores = new Button(
        [&](){
            return Vector2((*screenWidth)/2, 8*(*screenHeight)/11);
        },
        [](){
            return Vector2(200, 50);
        },
        UIPlacement::CENTER,
        "Placar de Lideres",
        [this](Button*){
            currentMenu = MenuState::HIGHSCORES;
            // fetch highscores
            bool success = false;
            auto res = WebManager::get(
                "ballsbounce.rodrigoappelt.com",
                "/highscores",
                {
                    { "maxEntries", "10"}
                },
                &success
            );
            if(success){
                highscoresBuffer.clear();
                auto arr = WebManager::fromJsonArray(res);
                for(auto obj : arr){
                    highscoresBuffer.push_back({obj["name"], std::stoi(obj["score"])});
                }
                highscoresFetched = true;
            }
        }
    );
    mainmenuhighscores->style = ButtonStyle::FlatLightBlue();
    mainMenuButtons.registerButton(mainmenuhighscores);

    auto mainmenustore = new Button(
        [&](){
            return Vector2((*screenWidth)/2, 9.5*(*screenHeight)/11);
        },
        [](){
            return Vector2(200, 50);
        },
        UIPlacement::CENTER,
        "Loja",
        [this](Button*){
            currentMenu = MenuState::STORE;
        }
    );
    mainmenustore->style = ButtonStyle::FlatLightBlue();
    mainMenuButtons.registerButton(mainmenustore);

    // IDENTIFICATION
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
        "CONTINUAR",
        [this](Button*){
            if(idTextBox->getText() == ""){
                return;
            }
            username = idTextBox->getText();
            PersistentStorage::set<std::string>("user","name",idTextBox->getText());
            currentMenu = MenuState::MAIN_MENU;
        }
    );
    idPlay->style = ButtonStyle::FlatGreen();
    idButtons.registerButton(idPlay);

    // pause buttons menu
    auto continuePauseMenu = new Button(
        [&](){
            return Vector2(*screenWidth/2, 2*(*screenHeight)/6);
        },
        [&](){
            return Vector2(*screenWidth/7, (*screenHeight)/8-10);
        },
        UIPlacement::CENTER,
        "CONTINUAR",
        [&](Button*){
            currentMenu = MenuState::GAME;
        }
    );
    continuePauseMenu->style = ButtonStyle::FlatRed();
    pauseButtons.registerButton(continuePauseMenu);

    auto retryPauseMenu = new Button(
        [&](){
            return Vector2(*screenWidth/2, 3*(*screenHeight)/6);
        },
        [&](){
            return Vector2(*screenWidth/7, (*screenHeight)/8-10);
        },
        UIPlacement::CENTER,
        "REINICIAR",
        [&](Button*){
            game.reset();
            currentMenu = MenuState::GAME;
        }
    );
    retryPauseMenu->style = ButtonStyle::FlatLightBlue();
    pauseButtons.registerButton(retryPauseMenu);

    auto mainMenuPauseMenu = new Button(
        [&](){
            return Vector2(*screenWidth/2, 4*(*screenHeight)/6);
        },
        [&](){
            return Vector2(*screenWidth/7, (*screenHeight)/8-10);
        },
        UIPlacement::CENTER,
        "MENU INICIAL\n(salvar)",
        [&](Button*){
            game.saveState();
            PersistentStorage::set("game", "hasSavedGame", 1);
            game.reset();
            currentMenu = MenuState::MAIN_MENU;
        }
    );
    mainMenuPauseMenu->style = ButtonStyle::FlatGreen();
    pauseButtons.registerButton(mainMenuPauseMenu);

    // game over menu
    auto mainMenuGameOver = new Button(
        [&](){
            return Vector2(2.5* *screenWidth/9, 6*(*screenHeight)/7 - 15);
        },
        [&](){
            return Vector2(1* *screenWidth/9, 0.5* *screenHeight/7);
        },
        UIPlacement::BOTTOM_CENTER,
        "MENU INICIAL",
        [&](Button*){
            game.reset();
            currentMenu = MenuState::MAIN_MENU;
        }
    );
    mainMenuGameOver->style = ButtonStyle::FlatGreen();
    gameOverButtons.registerButton(mainMenuGameOver);

    auto retryGameOver = new Button(
        [&](){
            return Vector2(6.5* *screenWidth/9, 6*(*screenHeight)/7 - 15);
        },
        [&](){
            return Vector2(1* *screenWidth/9, 0.5* *screenHeight/7);
        },
        UIPlacement::BOTTOM_CENTER,
        "REINICIAR",
        [&](Button*){
            game.reset();
            currentMenu = MenuState::GAME;
        }
    );
    retryGameOver->style = ButtonStyle::FlatLightBlue();
    gameOverButtons.registerButton(retryGameOver);
    // play again button
    // main menu button

    // HIGHSCORE MENU
    auto highscoreback = new Button(
        [&](){
            return Vector2(25);
        },
        [&](){
            return Vector2(75, 50);
        },
        UIPlacement::TOP_LEFT,
        "VOLTAR",
        [&](Button*){
            currentMenu = MenuState::MAIN_MENU;
        }
    );
    highscoreback->style = ButtonStyle::FlatRed();
    highscoresButtons.registerButton(highscoreback);
}

// ENDREGION UI
