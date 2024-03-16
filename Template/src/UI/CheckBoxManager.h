#ifndef __CHECKBOX_MANAGER_H__
#define __CHECKBOX_MANAGER_H__

#include <vector>

#include "CheckBox.h"

// manages many instances of the checkbox UI element
class CheckboxManager {
public:
    void registerCheckbox(Checkbox *button);
    void unregisterCheckbox(Checkbox *button);

    CheckboxManager();
    ~CheckboxManager();

    void draw();
    void updateMousePos(Vector2 mousePos);
    void mouseDown();
    void mouseUp();
private:
    Vector2 mousePos;
    Checkbox* holdingCheckbox;
    std::vector<Checkbox*> checkboxes;
};

#endif // __CHECKBOX_MANAGER_H__
