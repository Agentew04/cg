#include "SideBar.h"
#include "../UI/Button.h"
#include "../UI/Chart.h"
#include "../UI/CheckBox.h"

void SideBar::submitButtons(){
    int margin = 5;

    // botoes de carregar imagens
    Vector2 tripButtonSize = Vector2((this->size.x-(margin*4))/3 ,50);
    
    ButtonStyle *btnstl = ButtonStyle::Windows10();
    Button *ldbtn1 = new Button(
        pos + Vector2(margin,margin), 
        tripButtonSize, 
        "Load 1", 
        [&](Button* but){
            loadBmp(0);
            but->clickable = false;
    });
    ldbtn1->style = btnstl;
    this->uiManager->add(ldbtn1);


    Button *ldbtn2 = new Button(
        pos + Vector2(110,5), 
        tripButtonSize, 
        "Load 2", 
        [&](Button* but){
            loadBmp(1);
            but->clickable = false;
    });
    ldbtn2->style = btnstl;
    this->uiManager->add(ldbtn2);


    Button *ldbtn3 = new Button(
        pos + Vector2(215,5), 
        tripButtonSize, 
        "Load 3", 
        [&](Button* but){
            loadBmp(2);
            but->clickable = false;
    });
    ldbtn3->style = btnstl;
    this->uiManager->add(ldbtn3);



    // botoes r,g,b,y
    Vector2 quadButtonSize = Vector2((this->size.x-(margin*5))/4, 50);

    Button *rbtn = new Button(
        pos + Vector2(margin, tripButtonSize.y+margin*2), 
        quadButtonSize, 
        "R", 
        [&](Button* but){
            imgCanvas->requestChannel(ImageManipulation::Channel::RED, false);
    });
    rbtn->style = btnstl;
    this->uiManager->add(rbtn);


    Button *gbtn = new Button(
        pos + Vector2(quadButtonSize.x+margin*2, tripButtonSize.y+margin*2), 
        quadButtonSize, 
        "G", 
        [&](Button* but){
            imgCanvas->requestChannel(ImageManipulation::Channel::GREEN, false);
    });
    gbtn->style = btnstl;
    this->uiManager->add(gbtn);


    Button *bbtn = new Button(
        pos + Vector2(quadButtonSize.x*2+margin*3, tripButtonSize.y+margin*2), 
        quadButtonSize, 
        "B", 
        [&](Button* but){
            imgCanvas->requestChannel(ImageManipulation::Channel::BLUE, false);
    });
    bbtn->style = btnstl;
    this->uiManager->add(bbtn);


    Button *ybtn = new Button(
        pos + Vector2(quadButtonSize.x*3+margin*4, tripButtonSize.y+margin*2), 
        quadButtonSize, 
        "Y", 
        [&](Button* but){
            imgCanvas->requestChannel(ImageManipulation::Channel::RED, true);
    });
    ybtn->style = btnstl;
    this->uiManager->add(ybtn);



    // botoes de flip(h,v)
    Vector2 flipButtonSize = Vector2((this->size.x-(margin*3))/2, 50);

    Button *fhbtn = new Button(
        pos + Vector2(margin, tripButtonSize.y+margin*3+quadButtonSize.y), 
        flipButtonSize, 
        "Flip Horizontal", 
        [&](Button* but){
            imgCanvas->requestFlip(false);
    });
    fhbtn->style = btnstl;
    this->uiManager->add(fhbtn);


    Button *fvbtn = new Button(
        pos + Vector2(flipButtonSize.x+margin*2, tripButtonSize.y+margin*3+quadButtonSize.y), 
        flipButtonSize, 
        "Flip Vertical", 
        [&](Button* but){
            imgCanvas->requestFlip(true);
    });
    fvbtn->style = btnstl;
    this->uiManager->add(fvbtn);



    // histograma
    Chart *histogram = new Chart(
        pos + Vector2(margin, tripButtonSize.y+margin*4+quadButtonSize.y*2), 
        Vector2(this->size.x-margin*2, 150),
        Chart::ChartType::LINE
    );
    this->uiManager->add(histogram);


    // opcoes do histograma
    Vector2 histCheckboxSz = Vector2((this->size.x-(margin*5))/4, 20);

    Checkbox::Style *chkstl = Checkbox::Style::Windows10();
    Checkbox *rckbx = new Checkbox(
        pos + Vector2(margin, tripButtonSize.y+margin*5+quadButtonSize.y*2+150), 
        histCheckboxSz,
        "Red", 
        false);
    rckbx->style = chkstl;
    this->uiManager->add(rckbx);

    Checkbox *gckbx = new Checkbox(
        pos + Vector2(histCheckboxSz.x+margin*2, tripButtonSize.y+margin*5+quadButtonSize.y*2+150), 
        histCheckboxSz,
        "Green", 
        false);
    gckbx->style = chkstl;
    this->uiManager->add(gckbx);

    Checkbox *bckbx = new Checkbox(
        pos + Vector2(histCheckboxSz.x*2+margin*3, tripButtonSize.y+margin*5+quadButtonSize.y*2+150), 
        histCheckboxSz,
        "Blue", 
        false);
    bckbx->style = chkstl;
    this->uiManager->add(bckbx);

    Checkbox *yckbx = new Checkbox(
        pos + Vector2(histCheckboxSz.x*3+margin*4, tripButtonSize.y+margin*5+quadButtonSize.y*2+150), 
        histCheckboxSz,
        "Luminance", 
        false);
    yckbx->style = chkstl;
    this->uiManager->add(yckbx);

}