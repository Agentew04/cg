#ifndef __CHART_H__
#define __CHART_H__

#include <vector>
#include <map>

#include "../Vector2.h"
#include "../Vector3.h"

/*
    Classe para desenhar graficos de linha e ponto.
    Cada grafico pode ter multiplas series.
    Cada serie tem um vetor de pontos (x,y), max e mins e uma cor.
*/

class Chart {
public:

    enum class ChartType {
        LINE,
        BAR, // nao implementado ainda
        POINT
    };

    class Series {
    public:
        uint32_t *x;
        uint32_t *y;
        int elements;
        Vector3 color;
        Vector2 xBounds, yBounds;
        bool visible = false;
    };

    void draw();

    Chart(Vector2 pos, Vector2 size, ChartType type);
    ~Chart();

    ChartType type;
    std::vector<Series*> series;
    Vector2 pos,size;
    
private:
    void drawSeries(Series *series);
};

#endif // __CHART_H__
