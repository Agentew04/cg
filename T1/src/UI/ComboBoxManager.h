#ifndef __COMBO_BOX_MANAGER_H__
#define __COMBO_BOX_MANAGER_H__

#include <vector>

#include "ComboBox.h"
#include "../Vector2.h"

// manages many instances of the checkbox UI element
class ComboBoxManager {
public:
    void registerComboBox(ComboBox *button);
    void unregisterComboBox(ComboBox *button);

    ComboBoxManager();
    ~ComboBoxManager();

    void draw();
    void updateMousePos(Vector2 mousePos);
    void mouseDown();
    void mouseUp();
private:
    Vector2 mousePos;
    ComboBox* holdingComboBox;
    std::vector<ComboBox*> comboBoxes;
};

#endif // __CHECKBOX_MANAGER_H__
