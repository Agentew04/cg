#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <string>

#include "../Vector2.h"
#include "../Vector3.h"

class Button{
public:
    Button(Vector3 pos, Vector3 sz, std::string text, bool relative);
    ~Button();

    void draw();

    bool clickable;
private:

    Vector2 pos;
    Vector2 sz;
    std::string text;

    Vector3 background;
    Vector3 foreground;
    Vector3 borderColor;
};

#endif // __BUTTON_H__
