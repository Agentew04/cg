#ifndef __CURSOR_MANAGER_H__
#define __CURSOR_MANAGER_H__

#include <windows.h>
#include <map>

/// @brief Classe que gerencia a mudanca do icone do cursor.
/// Consegue carregar arquivos .cur e .ani. Utiliza
/// a Win32 API para fazer a mudanca do cursor.
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

    /// @brief Define qual o tipo de mouse que vai
    /// ser mostrado.
    /// @param type O tipo do cursor a ser mostrado
    static void setCursor(CursorType type);

    static void startFrame();

    /// @brief Faz a chamada para mudar o cursor
    static void applyCursor();

    /// @brief Libera todos os recursos usados pelos cursores
    static void freeResources();

private:

    static bool hasMouseSet;
    static CursorType currentType;
    static std::map<CursorType, bool> loaded;
    static std::map<CursorType, HCURSOR> cursors;

    static void loadCursor(CursorType type);
    static LPCTSTR cursorTypeToString(CursorType type);
};

#endif