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
    void click();
private:
    std::vector<Button> buttons;
};

#endif // __BUTTON_MANAGER_H__
