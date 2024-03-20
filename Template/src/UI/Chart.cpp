#include <vector>

#include "Chart.h"
#include "../gl_canvas2d.h"

Chart::Chart(Vector2 pos, Vector2 size){
    this->pos = pos;
    this->size = size;
}

Chart::~Chart(){
}


void Chart::draw(){
    CV::translate(this->pos);

    // draw axis
    CV::color(0,0,0);
    CV::line(0, this->size.y, this->size.x, this->size.y, 2.0); // horizontal
    CV::line(0, 0, 0, this->size.y, 2.0); // vertical

    // draw points
    int n = this->x.size();

    for(int i = 0; i < n; i++){
        float x = this->x[i];
        float y = this->y[i];

        //if(x < this->xBounds.x || x > this->xBounds.y || y < this->yBounds.x || y > this->yBounds.y){
        //    continue;
        //}

        // convert to screen coordinates
        // pos.x -> xBounds.x
        // pos.x + size.x -> xBounds.y
        float actualX = pos.x + ((xBounds.y - x) - x);
        float actualY = pos.y + ((yBounds.y - y) - y);

        CV::color(this->color);
        CV::circle(actualX * this->size.x, actualY * this->size.y, 5, 50);
    }
    
}


















