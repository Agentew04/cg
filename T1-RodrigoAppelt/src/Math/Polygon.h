#ifndef __POLYGON_H__
#define __POLYGON_H__

#include <vector>

#include "../Vector2.h"

class Polygon2D {
public:

    Polygon2D(){

    }
    Polygon2D(std::vector<Vector2> vertices){
        this->vertices = vertices;
    }
    Polygon2D(Polygon2D &p){
        this->vertices = p.vertices;
    }
    ~Polygon2D(){
        this->vertices.clear();
    }

    bool pointInside(Vector2 point){
        // Check for empty polygon
        int i, j, c = 0;
        for (i = 0, j = vertices.size()-1; i < vertices.size(); j = i++) {
            if ( ((vertices[i].y>point.y) != (vertices[j].y>point.y)) &&
            (point.x < (vertices[j].x-vertices[i].x) * (point.y-vertices[i].y) / (vertices[j].y-vertices[i].y) + vertices[i].x) )
            c = !c;
        }
        return c;
    }

    std::vector<Vector2> vertices;
private:

    inline int orientation(Vector2 p, Vector2 q, Vector2 r){
        int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
        if (val == 0) return 0;
        return (val > 0)? 1: 2;
    }
};

#endif