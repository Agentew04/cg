#include <algorithm>
#include <vector>

#include "Chart.h"
#include "../gl_canvas2d.h"

/**
 * implementacao de Chart.h
 * Documentacao esta no .h
*/

Chart::Chart(Vector2 pos, Vector2 size, Chart::ChartType type){
    this->pos = pos;
    this->size = size;
    this->type = type;
}

Chart::~Chart(){
    std::vector<uint32_t*> deleted;
    for(auto& series : this->series){
        if(std::find(deleted.begin(), deleted.end(), series->x) == deleted.end()){
            deleted.push_back(series->x);
            delete series->x;
        }
        if(std::find(deleted.begin(), deleted.end(), series->y) == deleted.end()){
            deleted.push_back(series->y);
            delete series->y;
        }
        delete series;
    }
    series.clear();
}

void Chart::draw(){
    CV::translate(this->pos);

    // draw axis
    CV::color(0,0,0);
    CV::line(0, this->size.y, this->size.x, this->size.y, 2.0); // horizontal
    CV::line(0, 0, 0, this->size.y, 2.0); // vertical

    // draw series
    for(auto& series : this->series){
        if(series->visible){
            drawSeries(series);
        }
    }
}

void Chart::drawSeries(Series *series){
    // draw points
    int n = series->elements;

    // stack allocated, n precisa dar free
    Vector2 actualCoords[1000]; // max points(histogram usa 256 pts)

    int plotted = 0;
    for(int i = 0; i < n; i++){
        float x = series->x[i];
        float y = series->y[i];

        // corta fora do range horizontal
        if(x < series->xBounds.x || x > series->xBounds.y){
            continue;
        }
        // e da clamp no range vertical
        if(y < series->yBounds.x){
            y = series->yBounds.x;
        }else if(y > series->yBounds.y){
            y = series->yBounds.y;
        }

        // inverte pq o y cresce pra baixo
        y = series->yBounds.y - y;

        float rateX = size.x / (series->xBounds.y - series->xBounds.x);
        float rateY = size.y / (series->yBounds.y - series->yBounds.x);

        float posX = (x - series->xBounds.x) * rateX;
        float posY = (y - series->yBounds.x) * rateY;
        actualCoords[i] = Vector2(posX, posY);

        CV::color(series->color);
        if(type == Chart::ChartType::POINT){
            CV::circleFill(posX, posY, 3, 50);
        }else if(type == Chart::ChartType::LINE){
            if(plotted > 0){
                CV::line(actualCoords[i-1].x, actualCoords[i-1].y, actualCoords[i].x, actualCoords[i].y, 2.0);
            }
        }
        plotted++;
    }
}
















