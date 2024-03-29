#ifndef __POLYGON_H__
#define __POLYGON_H__

#include <vector>

#include "../Vector2.h"

class Polygon {
public:

    Polygon(std::vector<Vector2> vertices){
        this->vertices = vertices;
    }
    Polygon(Polygon &p){
        this->vertices = p.vertices;
    }
    ~Polygon(){
        this->vertices.clear();
    }

    bool pointInside(Vector2 point){
        int prev_orient = 0;
        for (int i = 0; i < vertices.size(); i++) {
            int next_i = (i + 1) % vertices.size();
            int orient = orientation(vertices[i], vertices[next_i], point);
            
            if (i == 0 || orient != prev_orient) {
                return false;
            }
            prev_orient = orient;
        }
        return true;
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