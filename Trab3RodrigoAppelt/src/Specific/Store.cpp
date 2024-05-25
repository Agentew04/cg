#include "Store.h"

#include "vector"
#include "string"

#include "../3D/ObjLoader.h"
#include "../Storage/PersistentStorage.h"
#include "../Math/Vector3.h"
#include "../gl_canvas2d.h"


// em um mundo ideal substituiria isso por
// arquivos .asset
std::vector<Store::Skin> Store::skins = {
    {
        "Padrao",
        "./Trab3RodrigoAppelt/models/ball1model.3d",
        "ball1",
        Vector3(1, 1, 1),
        0,
        true
    },
    {
        "Quadrado",
        "./Trab3RodrigoAppelt/models/ball2model.3d",
        "ball2",
        Vector3(0, 1, 1),
        10,
        false
    },
    {
        "Triangulo",
        "./Trab3RodrigoAppelt/models/ball3model.3d",
        "ball3",
        Vector3(1, 0, 0),
        25,
        false
    },
    {
        "Pentagono",
        "./Trab3RodrigoAppelt/models/ball4model.3d",
        "ball4",
        Vector3(0, 1, 0),
        50,
        false
    },
    {
        "$$$",
        "./Trab3RodrigoAppelt/models/moeda.3d",
        "ball5",
        Vector3(1, 0, 0),
        100,
        false
    },
    {
        "Caveira",
        "./Trab3RodrigoAppelt/models/skull.3d",
        "ball6",
        Vector3(1, 1, 1),
        200,
        false
    }
};
Store* Store::instance = nullptr;

Store::Store(int *screenWidth, int *screenHeight, App* app) :
    screenWidth(screenWidth),
    screenHeight(screenHeight),
    app(app){
    if(instance == nullptr){
        instance = this;
    }
}

// forward declaration of app.goToMainMenu
void App::goToMainMenu();

void Store::load(){
    selectedSkinIndex = 0; // a default

    if(skins.size() == 0){
        std::cout << "ERROR: Sem skins para carregar. Pode dar tudo errado" << std::endl;
    }

    // carregar os modelos na engine
    for(auto &skin: skins){
        ObjLoader::load(skin.modelPath, skin.modelId);
        std::cout << "Carreguei " << skin.modelId << " em " << skin.modelPath << std::endl;
    }

    // ve se o usuario ja comprou alguma skin
    for(uint32_t i=0; i<skins.size(); i++){
        std::string key = "ball"+std::to_string(i)+"unlocked";
        bool has = PersistentStorage::has("store", key);

        if(has){
            skins[i].unlocked = PersistentStorage::get<bool>("store", key, false);
        } else {
            PersistentStorage::set<bool>("store", key, skins[i].unlocked);
        }
    }

    // cria os botoes
    for(uint32_t i=0; i<skins.size(); i++){
        auto buybtn = new Button(
            [&]() {
                return Vector2();
            },
            [](){
                return Vector2(100, 50);
            },
            UIPlacement::CENTER,
            "Comprar",
            [=,this](Button*){
                buySkin(i);
            }
        );
        buybtn->style= ButtonStyle::FlatRed();

        auto equipbtn = new Button(
            [&]() {
                return Vector2();
            },
            [](){
                return Vector2(100, 50);
            },
            UIPlacement::CENTER,
            "Equipar",
            [=,this](Button*){
                equipSkin(i);
            }
        );
        equipbtn->style = ButtonStyle::FlatLightBlue();
        buttons.push_back(Tuple<Button*, Button*>(buybtn, equipbtn));

        if(skins[i].unlocked){
            buttonManager.registerButton(equipbtn);
        } else {
            buttonManager.registerButton(buybtn);
        }
    }

    // cria botao de voltar
    auto storeback = new Button(
        [&](){
            return Vector2(25);
        },
        [&](){
            return Vector2(75, 50);
        },
        UIPlacement::TOP_LEFT,
        "VOLTAR",
        [this](Button*){
            app->goToMainMenu();
        }
    );
    storeback->style = ButtonStyle::FlatRed();
    buttonManager.registerButton(storeback);
}

std::string Store::getCurrentSkinModelIdentifier(){
    return skins[instance->selectedSkinIndex].modelId;
}

Vector3 Store::getCurrentSkinColor(){
    return skins[instance->selectedSkinIndex].color;
}

void Store::render(){
    CV::translate(Vector2::zero());
    float usableWidth = 3* *screenWidth / 5.0f;
    float minCardMargin = 15;

    float cardWidth = (usableWidth-minCardMargin*(cardColumns-1)) / cardColumns;
    float cardHeight = 400;

    Vector2 cardAreaStart = Vector2(1* *screenWidth/5.0f, 1* *screenHeight/5.0f);

    // renderiza as skins
    for(int i=0; i<skins.size(); i++){
        Vector2 cardIndex = Vector2(i % cardColumns, i / cardColumns);
        Vector2 cardPos = cardAreaStart;
        cardPos.x += cardIndex.x * (cardWidth + minCardMargin);
        cardPos.y += cardIndex.y * (cardWidth + minCardMargin);

        renderCard(i, cardPos, Vector2(cardWidth, cardHeight), minCardMargin);
    }
}

void Store::renderCard(int index, Vector2 pos, Vector2 cardSize, float margin){
    // renderiza o background(sombra + card)
    float shadowDropoff = 10;
    Vector3 shadowColor = Vector3::fromHex(0x242535);
    CV::color(shadowColor);
    CV::rectFill(pos + Vector2(shadowDropoff), pos + cardSize + Vector2(shadowDropoff));
    Vector3 cardColor = Vector3::fromHex(0x383947);
    CV::color(cardColor);
    CV::rectFill(pos, pos + cardSize);

    // renderiza o modelo
    CV::color(skins[index].color);
    CV::obj(
        ObjLoader::get(skins[index].modelId),
        pos + cardSize*0.5,
        Vector2(cardSize.x * 0.75f)
    );

    if(skins[index].unlocked){
        // renderiza o botao de equipar
    } else {
        // renderiza o botao de comprar
    }

    // renderiza o nome
    CV::color(1,1,1);
    CV::text(pos + Vector2(0, cardSize.y), skins[index].name, 20);
    // renderiza o preco
    CV::text(pos + Vector2(0, cardSize.y + 20), "$"+std::to_string(skins[index].price), 20);
}

void Store::buySkin(int index){
    if(skins[index].unlocked){
        return;
    }

    int coins = PersistentStorage::get<int>("store", "coins", 0);
    if(coins >= skins[index].price){
        PersistentStorage::set<int>("store", "coins", coins - skins[index].price);
        PersistentStorage::set<bool>("store", "ball"+std::to_string(index)+"unlocked", true);
        skins[index].unlocked = true;
        buttonManager.unregisterButton(buttons[index].first);
        buttonManager.registerButton(buttons[index].second);
    }
}

void Store::equipSkin(int index){
    if(!skins[index].unlocked){
        return;
    }

    selectedSkinIndex = index;
}

void Store::mouseDown(){
    buttonManager.mouseDown();
}

void Store::mouseUp(){
    buttonManager.mouseUp();
}

void Store::updateMousePos(Vector2 pos){
    buttonManager.updateMousePos(pos);
}
