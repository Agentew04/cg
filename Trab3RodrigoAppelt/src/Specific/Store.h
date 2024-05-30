#ifndef __STORE_H__
#define __STORE_H__

#include <vector>

#include "../Math/Vector2.h"
#include "../Math/Vector3.h"
#include "../UI/ButtonManager.h"
#include "../Misc/Tuple.h"
#include "../3D/ObjLoader.h"


/// @brief Classe que agrega a logica da compra de
/// skins para a bola
class Store {
public:
    Store(int *screenWidth, int *screenHeight);

    /// @brief Carrega os assets necessarios para a loja
    void load();

    static Model3D* getCurrentSkinModel();
    static Vector3 getCurrentSkinColor();

    bool wantToGoBack = false;

    // event forwarding
    void render();
    void mouseDown();
    void mouseUp();
    void updateMousePos(Vector2 pos);
private:
    static Store* instance;

    struct Skin{
        std::string name;
        std::string modelId;
        Vector3 color;
        int price;
        bool unlocked;
    };

    int *screenWidth, *screenHeight;

    int cardColumns = 3;
    void renderCard(int index, Vector2 pos, Vector2 cardSize);

    void buySkin(int index);
    void equipSkin(int index);
    static std::vector<Skin> skins;

    int selectedSkinIndex;

    std::vector<Tuple<Button*, Button*>> buttons;

    ButtonManager buttonManager;

    Vector2 getPosForCard(int i);
    Vector2 getCardSize();
};

#endif
