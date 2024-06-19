#ifndef __UI_DEFINITIONS_H__
#define __UI_DEFINITIONS_H__

#include "../Math/Vector2.h"

enum class UIPlacement{
    TOP_LEFT,
    LEFT,
    BOTTOM_LEFT,
    TOP_CENTER,
    CENTER,
    BOTTOM_CENTER,
    TOP_RIGHT,
    RIGHT,
    BOTTOM_RIGHT
};

enum class UIState {
    NORMAL,
    HOVER,
    CLICK,
    FOCUSED // pode ser desconsiderado em botoes etc
};

enum TextAlign {
    LEFT,
    CENTER,
    RIGHT
};

inline void translateCoordinates(Vector2& pos, Vector2& size, UIPlacement placement){
    if(placement == UIPlacement::TOP_LEFT){
        // ja esta normalizado
        return;
    }
    if(placement == UIPlacement::CENTER){
        pos = pos - size/2;
    }else if(placement == UIPlacement::TOP_CENTER){
        pos = pos - Vector2(size.x/2, 0);
    }else if(placement == UIPlacement::BOTTOM_CENTER){
        pos = pos - Vector2(size.x/2, size.y);
    }else if(placement == UIPlacement::LEFT){
        pos = pos - Vector2(0, size.y/2);
    }else if(placement == UIPlacement::RIGHT){
        pos = pos - Vector2(size.x, size.y/2);
    }else if(placement == UIPlacement::TOP_RIGHT){
        pos = pos - Vector2(size.x, 0);
    }else if(placement == UIPlacement::BOTTOM_LEFT){
        pos = pos - Vector2(0, size.y);
    }else if(placement == UIPlacement::BOTTOM_RIGHT){
        pos = pos - size;
    }
}

#endif