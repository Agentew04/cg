#ifndef __CURSOR_MANAGER_H__
#define __CURSOR_MANAGER_H__

#include <windows.h>
#include <map>

/// @brief Classe que gerencia a mudanca do icone do cursor
class CursorManager{
public:
    enum class CursorType {
        DEFAULT,
        CLICKABLE,
        MOVE,
        LOADING,
        RESIZE_HORIZONTAL,
        RESIZE_VERTICAL,
        RESIZE_DIAGONAL_1,
        RESIZE_DIAGONAL_2,
        ROTATE
    };

    static void setCursor(CursorType type);

    /// @brief Libera todos os recursos usados pelos cursores
    static void freeResources();

private:

    static std::map<CursorType, bool> loaded;
    static std::map<CursorType, HANDLE> cursors;

    static void loadCursor(CursorType type);
    static LPCTSTR cursorTypeToString(CursorType type);
};

#endif