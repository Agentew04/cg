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
}

void Chart::draw(){
    CV::translate(this->pos);

    // draw axis
    CV::color(0,0,0);
    CV::line(0, this->size.y, this->size.x, this->size.y, 2.0); // horizontal
    CV::line(0, 0, 0, this->size.y, 2.0); // vertical

    // draw series
    for(auto& series : this->series){
        drawSeries(series);
    }
}

void Chart::drawSeries(Series& series){
    // draw points
    int n = series.elements;

    // stack allocated, n precisa dar free
    Vector2 actualCoords[1000]; // max points(histogram usa 256 pts)

    for(int i = 0; i < n; i++){
        float x = series.x[i];
        float y = series.y[i];

        if(x < series.xBounds.x || x > series.xBounds.y || y < series.yBounds.x || y > series.yBounds.y){
            continue;
        }
        y = series.yBounds.y - y;

        // convert to screen coordinates
        // pos.x -> xBounds.x
        // pos.x + size.x -> xBounds.y
        float rateX = size.x / (series.xBounds.y - series.xBounds.x);
        float rateY = size.y / (series.yBounds.y - series.yBounds.x);

        float posX = (x - series.xBounds.x) * rateX;
        float posY = (y - series.yBounds.x) * rateY;
        actualCoords[i] = Vector2(posX, posY);

        CV::color(series.color);
        if(type == Chart::ChartType::POINT){
            CV::circleFill(posX, posY, 3, 50);
        }else if(type == Chart::ChartType::LINE){
            if(i > 0){
                CV::line(actualCoords[i-1].x, actualCoords[i-1].y, actualCoords[i].x, actualCoords[i].y, 2.0);
            }
        }
    }
}
















