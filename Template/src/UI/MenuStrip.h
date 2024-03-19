#ifndef __MENU_STRIP_H__
#define __MENU_STRIP_H__

#include <string>
#include <functional>

// classe que gerencia uma barra de ferramentas
class MenuStrip {
public:
    void addOption(std::string text, std::function<void()> callback);

    void draw();
private:

};

#endif // __MENU_STRIP_H__
