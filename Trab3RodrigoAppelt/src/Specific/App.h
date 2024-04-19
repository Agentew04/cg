#ifndef __APP_H__
#define __APP_H__

#include "../Math/Vector2.h"
#include "Game.h"
#include "../Keyboard.h"
#include "../UI/ButtonManager.h"
#include "../UI/TextBox.h"

/// @brief Classe que vai guardar o estado atual
/// da aplicacao.(qual menu esta, os dados do jogo,
/// usuario, etc). 
/// Tambem serve de "desambiguacao" para os eventos
/// (encaminha para o gerenciador da tela atual).
class App {
public:
    /// @brief Cria uma instancia do aplicativo
    App(int *scrW, int *scrH);
    /// @brief Libera recursos do aplicativo
    ~App();

    // event forwarding
    void update(float delta);
    void render();
    void keyDown(Key key);
    void keyUp(Key key);
    void mouseUp();
    void mouseDown();
    void updateMousePos(Vector2 pos);

private:
    enum class MenuState{
        MAIN_MENU,
        IDENTIFICATION,
        //HOW_TO_PLAY, // ignorar
        //CONFIG, // ignorar?
        //SHOP, // ignorar
        //RATING, // ignorar
        GAME,
        PAUSED,
        GAME_OVER,
        POST_GAME_STATS
    };
    MenuState currentMenu;

    int *screenWidth;
    int *screenHeight;
    Vector2 mousePos;

    Game game;

    /// @brief Cria os botoes de todos os menus
    void submitButtons();

    // render functions
    void renderMainMenu();
    void renderIdentification();
    void renderGame();
    void renderPauseMenu();
    void renderGameOver();
    void renderPostGameStats();

    // per screen variables
    ButtonManager mainMenuButtons;

    TextBox *idTextBox;
    ButtonManager idButtons;

    ButtonManager pauseButtons;
    ButtonManager gameButtons;
    ButtonManager gameOverButtons;
    ButtonManager postGameStatsButtons;

    // user data
    // int coins; <-     precisamos aqui? talvez soh no 
    // int highscore;    persistent storage ja serve!
    std::string username;
};

#endif