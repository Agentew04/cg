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
        "ball1",
        Vector3(1, 1, 1),
        0,
        true
    },
    {
        "Quadrado",
        "ball2",
        Vector3::fromHex(0x2db7fc),
        10,
        false
    },
    {
        "Triangulo",
        "ball3",
        Vector3::fromHex(0xed4242),
        25,
        false
    },
    {
        "Pentagono",
        "ball4",
        Vector3::fromHex(0x09ba2a),
        50,
        false
    },
    {
        "Dinheiro",
        "ball5",
        Vector3::fromHex(0xfcba03),
        100,
        false
    },
    {
        "Caveira",
        "ball6",
        Vector3::fromHex(0xd9dbdb),
        200,
        false
    }
};
Store* Store::instance = nullptr;

Store::Store(int *screenWidth, int *screenHeight) :
    screenWidth(screenWidth),
    screenHeight(screenHeight){
    if(instance == nullptr){
        instance = this;
    }
}

void Store::load(){
    selectedSkinIndex = PersistentStorage::getOrSetDefault<int>("store", "selectedSkin", 0);
    if(selectedSkinIndex >= skins.size()){
        selectedSkinIndex = 0;
        PersistentStorage::set<int>("store", "selectedSkin", selectedSkinIndex);
    }

    if(skins.size() == 0){
        std::cout << "ERROR: Sem skins para carregar. Pode dar tudo errado" << std::endl;
    }

    // carregar os modelos na engine
    ObjLoader::load(".\\Trab3RodrigoAppelt\\models\\balls.3d", "ballAssets");

    // ve se o usuario ja comprou alguma skin
    for(uint32_t i=0; i<skins.size(); i++){
        std::string key = "ball"+std::to_string(i)+"unlocked";
        bool has = PersistentStorage::has("store", key);

        if(has){
            bool value = PersistentStorage::get<bool>("store", key, false);
            if(i == selectedSkinIndex && !value){
                // anti-cheat primitivo
                selectedSkinIndex = 0;
                PersistentStorage::set<int>("store", "selectedSkin", selectedSkinIndex);
            }
            skins[i].unlocked = value;
        } else {
            PersistentStorage::set<bool>("store", key, skins[i].unlocked);
        }
    }

    // cria os botoes
    for(uint32_t i=0; i<skins.size(); i++){
        auto buybtn = new Button(
            [=]() {
                Vector2 cardPos = Store::getPosForCard(i);
                Vector2 cardSize = Store::getCardSize();
                return cardPos + cardSize.multiply(Vector2(0.5f, 0.8f));
            },
            [](){
                return Vector2(100, 40);
            },
            UIPlacement::CENTER,
            "Comprar",
            [=,this](Button*){
                buySkin(i);
            }
        );
        buybtn->style= ButtonStyle::FlatRed();

        auto equipbtn = new Button(
            [=]() {
                Vector2 cardPos = Store::getPosForCard(i);
                Vector2 cardSize = Store::getCardSize();
                return cardPos + cardSize.multiply(Vector2(0.5f, 0.8f));
            },
            [](){
                return Vector2(100, 40);
            },
            UIPlacement::CENTER,
            "Equipar",
            [=,this](Button*){
                equipSkin(i);
            }
        );
        equipbtn->style = ButtonStyle::FlatLightBlue();
        buttons.push_back(Tuple<Button*, Button* >(buybtn, equipbtn));

        if(skins[i].unlocked){
            if(i != selectedSkinIndex){
                buttonManager.registerButton(equipbtn);
            }
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
            wantToGoBack = true;
        }
    );
    storeback->style = ButtonStyle::FlatRed();
    buttonManager.registerButton(storeback);
}

Model3D* Store::getCurrentSkinModel(){
    return ObjLoader::get("ballAssets", skins[instance->selectedSkinIndex].modelId);
}

Vector3 Store::getCurrentSkinColor(){
    return skins[instance->selectedSkinIndex].color;
}

void Store::render(){
    CV::translate(Vector2::zero());
    CV::clear(Vector3::fromHex(0x1D1E30));

    // renderiza a quantidade de moedas em cima
    int coins = PersistentStorage::get<int>("user", "coins", 0);
    CV::color(Vector3::fromHex(0xfcba03));
    CV::text(Vector2(*screenWidth*0.5f, 0.5f * *screenHeight / 5.0f), "Moedas: "+std::to_string(coins), 20);

    // renderiza as skins
    for(int i=0; i<skins.size(); i++){
        renderCard(i, getPosForCard(i), getCardSize());
    }

    buttonManager.draw();
}

void Store::renderCard(int index, Vector2 pos, Vector2 cardSize){
    CV::translate(pos);

    // renderiza o background(sombra + card)
    float shadowDropoff = 10;
    Vector3 shadowColor = Vector3::fromHex(0x242535);
    CV::color(shadowColor);
    CV::rectFill(Vector2(shadowDropoff), cardSize + Vector2(shadowDropoff));
    Vector3 cardColor = Vector3::fromHex(0x383947);
    CV::color(cardColor);
    CV::rectFill(Vector2::zero(), cardSize);

    // renderiza o modelo
    CV::color(skins[index].color);
    CV::obj(
        ObjLoader::get("ballAssets", skins[index].modelId),
        Vector2(
            cardSize.x * 0.5f,
            cardSize.y * 0.25f
        ),
        Vector2(cardSize.x * 0.1f)
    );

    // botoes sao renderizados pelo buttonManager

    // renderiza o nome da skin
    CV::color(1,1,1);
    CV::text(
        Vector2(
            cardSize.x * 0.5f, 
            cardSize.y * 0.5f
        ), 
        skins[index].name, 
        20
    );
    // renderiza o preco
    CV::text(
        Vector2(
            cardSize.x * 0.5f, 
            cardSize.y * 0.6f
        ), 
        "$"+std::to_string(skins[index].price), 
        20
    );
}

void Store::buySkin(int index){
    if(skins[index].unlocked){
        return;
    }

    int coins = PersistentStorage::get<int>("user", "coins", 0);
    if(coins >= skins[index].price){
        PersistentStorage::set<int>("user", "coins", coins - skins[index].price);
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

    // registra dnv o botao de equipar da skin antiga
    buttonManager.registerButton(buttons[selectedSkinIndex].second);

    selectedSkinIndex = index;

    PersistentStorage::set<int>("store", "selectedSkin", selectedSkinIndex);

    // desregistra o botao de equipar dessa skin
    buttonManager.unregisterButton(buttons[index].second);
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

Vector2 Store::getPosForCard(int i){
    float minCardMargin = 15;
    auto cardSize = getCardSize();
    Vector2 cardAreaStart = Vector2(1* *screenWidth/5.0f, 1* *screenHeight/5.0f);

    Vector2 cardIndex = Vector2(i % cardColumns, i / cardColumns);
    Vector2 cardPos = cardAreaStart;
    cardPos.x += cardIndex.x * (cardSize.x + minCardMargin);
    cardPos.y += cardIndex.y * (cardSize.y + minCardMargin);

    return cardPos;
}

Vector2 Store::getCardSize(){
    float usableWidth = 3* *screenWidth / 5.0f;
    float minCardMargin = 15;

    float cardWidth = (usableWidth-minCardMargin*(cardColumns-1)) / cardColumns;
    float cardHeight = 200;
    return Vector2(cardWidth, cardHeight);
}