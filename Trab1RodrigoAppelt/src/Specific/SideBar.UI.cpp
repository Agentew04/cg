#include <string.h>
#include <limits>
#include <string>

#include "SideBar.h"
#include "../UI/Button.h"
#include "../UI/Chart.h"
#include "../UI/CheckBox.h"
#include "../Storage/PersistentStorage.h"

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
}

void SideBar::submitHistogram(){
    int margin = 5;
    Vector2 tripButtonSize = Vector2((this->size.x-(margin*4))/3 ,50);
    Vector2 quadButtonSize = Vector2((this->size.x-(margin*5))/4, 50);


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
    redCheckbox = new Checkbox(
        pos + Vector2(margin, tripButtonSize.y+margin*5+quadButtonSize.y*2+150), 
        histCheckboxSz,
        "Red", 
        false);
    redCheckbox->style = chkstl;
    this->uiManager->add(redCheckbox);
    redCheckbox->setCallback([&](bool value){
        PersistentStorage::setInt("sidebar", "histRVisible", value);
        //imgCanvas->updateSelectedHistograms();
    });

    greenCheckbox = new Checkbox(
        pos + Vector2(histCheckboxSz.x+margin*2, tripButtonSize.y+margin*5+quadButtonSize.y*2+150), 
        histCheckboxSz,
        "Green", 
        false);
    greenCheckbox->style = chkstl;
    this->uiManager->add(greenCheckbox);
    greenCheckbox->setCallback([&](bool value){
        PersistentStorage::setInt("sidebar", "histGVisible", value);
    });

    blueCheckbox = new Checkbox(
        pos + Vector2(histCheckboxSz.x*2+margin*3, tripButtonSize.y+margin*5+quadButtonSize.y*2+150), 
        histCheckboxSz,
        "Blue", 
        false);
    blueCheckbox->style = chkstl;
    this->uiManager->add(blueCheckbox);
    blueCheckbox->setCallback([&](bool value){
        PersistentStorage::setInt("sidebar", "histBVisible", value);
    });

    luminanceCheckbox = new Checkbox(
        pos + Vector2(histCheckboxSz.x*3+margin*4, tripButtonSize.y+margin*5+quadButtonSize.y*2+150), 
        histCheckboxSz,
        "Luminance", 
        false);
    luminanceCheckbox->style = chkstl;
    this->uiManager->add(luminanceCheckbox);
    luminanceCheckbox->setCallback([&](bool value){
        PersistentStorage::setInt("sidebar", "histLumVisible", value);
    });

    Slider::Style *slstl = Slider::Style::Windows10();
    this->histogramMaxSlider = new Slider(
        pos + Vector2(margin, tripButtonSize.y+margin*5+quadButtonSize.y*2+150+20), 
        Vector2(this->size.x-margin*2, 20),
        std::numeric_limits<float>::min(), 
        1.0f, 
        0.75f,
        Slider::Orientation::HORIZONTAL
    );
    this->histogramMaxSlider->setCallback([&](float value){
        imgCanvas->updateSelectedHistograms();
    });
    this->histogramMaxSlider->style = slstl;
    this->uiManager->add(this->histogramMaxSlider);
    
    // alocar os arrays dos histogramas
    auto histRArr = new uint32_t[UINT8_MAX+1];
    auto histGArr = new uint32_t[UINT8_MAX+1];
    auto histBArr = new uint32_t[UINT8_MAX+1];
    auto histLumArr = new uint32_t[UINT8_MAX+1];
    memset(histRArr, 0, sizeof(uint32_t)*(UINT8_MAX+1));
    memset(histGArr, 0, sizeof(uint32_t)*(UINT8_MAX+1));
    memset(histBArr, 0, sizeof(uint32_t)*(UINT8_MAX+1));
    memset(histLumArr, 0, sizeof(uint32_t)*(UINT8_MAX+1));

    // criar as series do grafico
    uint32_t *sharedX = new uint32_t[UINT8_MAX+1];
    for(int i = 0; i < UINT8_MAX+1; i++){
        sharedX[i] = i;
    }
    histR = new Chart::Series;
    histG = new Chart::Series;
    histB = new Chart::Series;
    histLum = new Chart::Series;

    histR->xBounds = histG->xBounds = histB->xBounds = histLum->xBounds = Vector2(0, UINT8_MAX);
    histR->yBounds = histG->yBounds = histB->yBounds = histLum->yBounds = Vector2(0, 1000);
    histR->elements = histG->elements = histB->elements = histLum->elements = UINT8_MAX+1;
    histR->x = histG->x = histB->x = histLum->x = sharedX;

    histR->y = histRArr;
    histG->y = histGArr;
    histB->y = histBArr;
    histLum->y = histLumArr;

    histR->color = Vector3(1,0,0);
    histG->color = Vector3(0,1,0);
    histB->color = Vector3(0,0,1);
    histLum->color = Vector3(0.5f, 0.5f, 0.5f);

    histogram->series.push_back(histR);
    histogram->series.push_back(histG);
    histogram->series.push_back(histB);
    histogram->series.push_back(histLum);
    
    redCheckbox->setBinding(&histR->visible);
    greenCheckbox->setBinding(&histG->visible);
    blueCheckbox->setBinding(&histB->visible);
    luminanceCheckbox->setBinding(&histLum->visible);

    this->gaussianSlider = new Slider(
        pos + Vector2(margin, tripButtonSize.y+margin*5+quadButtonSize.y*2+150+20+20+20), 
        Vector2(this->size.x-margin*2, 20),
        0.0f, 
        20.0f,
        0.0f,
        Slider::Orientation::HORIZONTAL
    );
    this->gaussianSlider->style = slstl;
    this->uiManager->add(this->gaussianSlider);
    this->gaussianSlider->setCallback([&](float value){
        this->brighnessSlider->setValue(0, false);
        this->contrastSlider->setValue(1, false);
        imgCanvas->updateGaussian(value);
    });

    this->brighnessSlider = new Slider(
        pos + Vector2(margin, tripButtonSize.y+margin*5+quadButtonSize.y*2+150+20*5), 
        Vector2(this->size.x-margin*2, 20),
        0.0f, 
        255.0f, 
        0.0f,
        Slider::Orientation::HORIZONTAL
    );
    this->brighnessSlider->style = slstl;
    this->uiManager->add(this->brighnessSlider);
    this->brighnessSlider->setCallback([&](float value){
        this->contrastSlider->setValue(1, false);
        this->gaussianSlider->setValue(0, false);
        imgCanvas->updateBrightness(value);
    });

    this->contrastSlider = new Slider(
        pos + Vector2(margin, tripButtonSize.y+margin*5+quadButtonSize.y*2+150+20*7),
        Vector2(this->size.x-margin*2, 20),
        0.0f,
        2.0f,
        1.0f,
        Slider::Orientation::HORIZONTAL
    );
    this->contrastSlider->style = slstl;
    this->uiManager->add(this->contrastSlider);
    this->contrastSlider->setCallback([&](float value){
        this->brighnessSlider->setValue(0, false);
        this->gaussianSlider->setValue(0, false);
        imgCanvas->updateContrast(value);
    });

}

void SideBar::linkImageCanvas(){
    imgCanvas->setHistograms(histR, histG, histB, histLum);
    histogramMaxSlider->setBinding(&imgCanvas->maxHistogramValueRatio);
    load();
}