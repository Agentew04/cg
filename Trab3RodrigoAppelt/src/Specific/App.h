#ifndef __APP_H__
#define __APP_H__

#include <vector>
#include <string>

#include "../Math/Vector2.h"
#include "Game.h"
#include "../Keyboard.h"
#include "../UI/ButtonManager.h"
#include "../UI/TextBox.h"
#include "../Misc/Tuple.h"

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
        MAIN_MENU, // feito
        IDENTIFICATION, // feito
        //HOW_TO_PLAY, // ignorar
        //CONFIG, // ignorar?
        //SHOP, // ignorar
        //RATING, // ignorar
        GAME, // feito
        PAUSED,
        GAME_OVER
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

    // user data
    // int coins; <-     precisamos aqui? talvez soh no
    // int highscore;    persistent storage ja serve!
    std::string username;

    std::vector<Tuple<std::string, int>> highscoresBuffer;
    bool highscoresFetched = false;
};

#endif
