#ifndef __BUTTON_MANAGER_H__
#define __BUTTON_MANAGER_H__

#include <vector>

#include "Button.h"

/** \brief Class to manage the drawing, and clicking of multiple buttons
 */
class ButtonManager {
public:
    void registerButton(Button button);
    void unregisterButton(Button button);

    ButtonManager();
    ~ButtonManager();

    void draw();
    void updateMousePos(Vector2 mousePos);
    void mouseDown();
    void mouseUp();
private:
    Vector2 mousePos;
    Button* holdingButton;
    std::vector<Button> buttons;
};

#endif // __BUTTON_MANAGER_H__
